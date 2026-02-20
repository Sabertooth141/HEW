//
// Created by saber on 1/14/2026.
//

#include "PlayerController.h"

#include <memory>

#include "../../../Game/Game.h"
#include "../../../Game/Scenes/GameScene.h"
#include "../../../Systems/TimeManager.h"
#include "../../../Lib/conioex_custom.h"
#include "../../../Systems/GameManager.h"
#include "../../../Util/UIManager.h"

PlayerController::PlayerController() : walkSpeed(0), normalDashSpeed(0), currDashSpeed(0),
                                       jumpForce(0),
                                       timeStopDuration(0),
                                       airResistance(0), normalGravity(0), isDamageable(true), dashTimer(0),
                                       normalDashDuration(0),
                                       currDashDuration(0),
                                       isDashing(false),
                                       snapshot(),
                                       normalStateMachine(PlayerNormalState::DEFAULT), attackController(),
                                       animatorPlaying(nullptr), rewindSnapshot()
{
}

void PlayerController::Initialize(const PlayerConfig& config, const PlayerAttackConfig& attackConfig)
{
    Entity::Initialize(config);

    walkSpeed = config.walkSpeed;
    jumpForce = config.jumpForce;
    timeStopDuration = config.timeStopDuration;
    airResistance = config.airResistance;
    normalGravity = config.gravity;
    normalDashDuration = config.dashDuration;
    normalDashSpeed = config.dashSpeed;

    trailFadeDuration = config.trailFadeDuration;

    attackController.Initialize(attackConfig, this);
}

void PlayerController::Start()
{
    Entity::Start();

    attackController.LoadAttackDuration();
    currSpeedX = walkSpeed;
    normalStateMachine.ChangeState(PlayerNormalState::IDLE);
    animatorPlaying = playerAnimators.GetAnimator(normalStateMachine.GetCurrState());
    animatorPlaying->Play();
}

void PlayerController::Update(const float deltaTime, const float trueDeltaTime, Tilemap& tileMap)
{
    if (TimeManager::Instance().IsRewinding())
    {
        if (input.timeRewind.IsPressed())
        {
            PlayerSnapshot rewind;
            if (TimeManager::Instance().UpdateRewind(deltaTime, rewind))
            {
                transform = rewind.transform;
                transform.CalculateCenterPosition();
                isFacingRight = rewind.isFacingRight;
                rewindSnapshot = rewind;

                if (currHp < rewind.currHp)
                {
                    currHp = rewind.currHp;
                }
            }
        }
        else
        {
            bool doAttack = false;
            TimeManager::RewindAttackFrame target{};
            TimeManager::Instance().EndRewind(doAttack, target);
            attackController.CancelCombo();
            if (doAttack)
            {
                TriggerRewindAttack(target);
            }
        }
        return;
    }

    HandleInput(deltaTime);
    attackController.Update(deltaTime, transform, isFacingRight);
    HandleAnimationUpdate(deltaTime);

    if (isInvic)
    {
        flickerCounter++;

        if (flickerCounter % flickerInterval == 0)
        {
            flickerVisible = !flickerVisible;
        }

    }
    else
    {
        flickerVisible = true;
    }

    Entity::Update(deltaTime, trueDeltaTime, tileMap);

    if (!isKnockedBack && normalStateMachine.GetCurrState() == PlayerNormalState::KNOCKBACK)
    {
        normalStateMachine.ChangeState(PlayerNormalState::IDLE);
    }

    if (trailFadeTimer > 0)
    {
        trailFadeTimer -= deltaTime;
    }

    snapshot.transform = transform;
    snapshot.isFacingRight = isFacingRight;
    snapshot.frame = animatorPlaying ? animatorPlaying->GetCurrentFrameBmp() : nullptr;
    snapshot.currHp = currHp;

    if (!TimeManager::Instance().IsRewinding())
    {
        TimeManager::Instance().RecordPlayerSnapshot(snapshot);
    }
}

void PlayerController::Draw(Camera& cam)
{
    DrawTrail(cam);

    if (TimeManager::Instance().IsRewinding() && !TimeManager::Instance().IsRewindAttack())
    {
        DrawRewind(cam);
        if (rewindSnapshot.frame != nullptr)
        {
            int screenX = cam.WorldToScreenX(transform.topLeft.x);
            int screenY = cam.WorldToScreenY(transform.topLeft.y);
            if (rewindSnapshot.isFacingRight)
            {
                DrawBmp(screenX, screenY, rewindSnapshot.frame);
            }
            else
            {
                DrawBmp(screenX, screenY, rewindSnapshot.frame, BMP_HINV);
            }
        }
        return;
    }

    if (animatorPlaying != nullptr)
    {
        if (flickerVisible)
        {
            animatorPlaying->Draw(cam, transform.topLeft.x, transform.topLeft.y, !isFacingRight);
        }
    }

    UIManager::Instance().DrawMovementIndic(-35, 5);

    if (TimeManager::Instance().CanTimeStop())
    {
        UIManager::Instance().DrawTimeStopIndic(GameConfig::VIEW_WIDTH - 210, -10);
    }

    if (TimeManager::Instance().CanTimeRewind())
    {
        UIManager::Instance().DrawTimeRewindIndic(GameConfig::VIEW_WIDTH - 210, 15);
    }

    // const PlayerSnapshot holoSnapshot = TimeManager::Instance().GetPlayerSnapshot();
    // int screenX = cam.WorldToScreenX(holoSnapshot.x);
    // int screenY = cam.WorldToScreenY(holoSnapshot.y);
    // DrawRect(screenX, screenY, screenX + transform.size.x, screenY + transform.size.y, RED, false);

    // Hitbox& hitbox = attackController.GetHitBox();
    // if (hitbox.isActive)
    // {
    //     int screenX = cam.WorldToScreenX(hitbox.transform.topLeft.x);
    //     int screenY = cam.WorldToScreenY(hitbox.transform.topLeft.y);
    //
    //     DrawRect(screenX, screenY, screenX + hitbox.transform.size.x, screenY + hitbox.transform.size.y, LIGHTRED,
    //              false);
    // }
}

void PlayerController::Die()
{
    Entity::Die();

    GameManager::Instance().UnregisterPlayer();
}

void PlayerController::InitAnimation(const PlayerNormalAnimPaths& path)
{
    std::unique_ptr<Animator> animator = std::make_unique<Animator>();
    animator->LoadSpriteSheet(path.jsonPath.c_str(), path.bmpPath.c_str());
    animator->SetLoopStartFrame(path.startFrame);

    playerAnimators.AddAnimator(path.animationState, std::move(animator));
}

void PlayerController::InitAttackAnimation(const PlayerCombatAnimPaths& path)
{
    std::unique_ptr<Animator> animator = std::make_unique<Animator>();
    animator->LoadSpriteSheet(path.jsonPath.c_str(), path.bmpPath.c_str());
    animator->SetLoopStartFrame(path.startFrame);

    playerAnimators.AddAnimator(path.animationState, std::move(animator));
}

void PlayerController::Dash(const float dashVel, const float inDashDuration, const bool isInvic)
{
    isDamageable = !isInvic;
    currDashDuration = inDashDuration;

    currDashSpeed = dashVel;
    isDashing = true;
    isDrawTrail = true;
}

void PlayerController::TakeDamage(const float inDamage)
{
    Entity::TakeDamage(inDamage);
}

void PlayerController::SetDamageable(const bool inDamageable)
{
    isDamageable = inDamageable;
}

SpriteSheet* PlayerController::GetSpriteSheetFromAnimator(const PlayerCombatState attkState)
{
    const Animator* resAnimator = playerAnimators.GetAnimator(attkState);
    return resAnimator->GetSpriteSheet();
}

void PlayerController::HandleMovement(const float deltaTime, Tilemap& tileMap)
{
    if (isGrounded && !isDashing && !isKnockedBack)
    {
        if (!isMovingInput && !attackController.IsRewindMoving())
        {
            normalStateMachine.ChangeState(PlayerNormalState::IDLE);
            velX = 0;
        }
    }
    else if (!isKnockedBack)
    {
        velX = velX + (0 - velX) * airResistance * deltaTime;

        if (velX > walkSpeed)
        {
            velX = walkSpeed;
        }
        else if (velX < -walkSpeed)
        {
            velX = -walkSpeed;
        }

        if (fabs(velX) < 0.1f)
        {
            velX = 0;
        }
    }

    if (!isDashing && !isKnockedBack)
    {
        if (!attackController.CanMove() && !attackController.IsRewindMoving())
        {
            currSpeedX = SPEED_WHEN_ATTK;
        }
        else
        {
            currSpeedX = walkSpeed;
            switch (isFacingRightBuffer)
            {
            case 1:
                isFacingRight = true;
                isFacingRightBuffer = -1;
                break;
            case 0:
                isFacingRight = false;
                isFacingRightBuffer = -1;
                break;
            default:
                break;
            }
        }
    }
    else if (isDashing)
    {
        currSpeedX = currDashSpeed;
        if (isFacingRight)
        {
            velX = currSpeedX;
        }
        else
        {
            velX = -currSpeedX;
        }

        if (dashTimer <= currDashDuration)
        {
            dashTimer += deltaTime;
        }
        else
        {
            if (attackController.GetCurrState() != PlayerCombatState::REWIND_ATTK)
            {
                isDamageable = true;
            }
            dashTimer = 0;
            isDashing = false;
            trailFadeTimer = trailFadeDuration;
            isDrawTrail = false;
        }
    }

    Entity::HandleMovement(deltaTime, tileMap);
}

void PlayerController::ApplyPhysics(const float deltaTime)
{
    if (!attackController.CanMove())
    {
        velY = 0;
        gravity = GRAV_WHEN_ATTK;
    }
    else
    {
        gravity = normalGravity;
    }

    Entity::ApplyPhysics(deltaTime);
}

void PlayerController::TakeKnockback(const float knockBackForce, const bool knockBackDirection)
{
    if (!isDamageable)
    {
        return;
    }

    normalStateMachine.ChangeState(PlayerNormalState::KNOCKBACK);

    Entity::TakeKnockback(knockBackForce, knockBackDirection);
}

void PlayerController::HandleInput(const float deltaTime)
{
    isMovingInput = false;

    if (isKnockedBack)
    {
        return;
    }

    if (TimeManager::Instance().IsRewindAttack())
    {
        return;
    }

    if (input.moveLeft.IsPressed())
    {
        if (isDashing)
        {
            return;
        }

        isMovingInput = true;

        if (attackController.CanMove() || attackController.IsInRecovery())
        {
            isFacingRight = false;
        }
        else if (!attackController.IsRewindMoving())
        {
            isFacingRightBuffer = 0;
        }

        velX = -currSpeedX;
        normalStateMachine.ChangeState(PlayerNormalState::MOVE);
    }
    else if (input.moveRight.IsPressed())
    {
        if (isDashing)
        {
            return;
        }

        isMovingInput = true;

        if (attackController.CanMove() || attackController.IsInRecovery())
        {
            isFacingRight = true;
        }
        else if (!attackController.IsRewindMoving())
        {
            isFacingRightBuffer = 1;
        }

        velX = currSpeedX;
        normalStateMachine.ChangeState(PlayerNormalState::MOVE);
    }

    if (input.jump.IsEdge() && isGrounded)
    {
        if (isDashing)
        {
            return;
        }

        if (attackController.IsAttacking())
        {
            return;
        }

        if (attackController.IsInRecovery())
        {
            attackController.CancelCombo();
        }

        velY = -jumpForce;
        normalStateMachine.ChangeState(PlayerNormalState::JUMP);
    }

    if (input.attack.IsEdge())
    {
        attackController.TryAttack();
    }

    if (input.timeStop.IsEdge())
    {
        if (TimeManager::Instance().IsTimeStopped())
        {
            TimeManager::Instance().DeactivateTimeStop();
        }
        else
        {
            TimeManager::Instance().ActivateTimeStop(timeStopDuration);
        }
    }

    if (input.timeRewind.IsEdge())
    {
        TimeManager::Instance().StartRewind();
    }
}

void PlayerController::TriggerRewindAttack(const TimeManager::RewindAttackFrame target)
{
    if (target.targetEnemy == nullptr)
    {
        return;
    }

    isFacingRightBuffer = -1;
    velX = 0;
    velY = 0;
    attackController.StartRewindAttack(target.targetEnemy);
}

void PlayerController::HandleAnimationUpdate(const float deltaTime)
{
    if (attackController.IsInRecovery())
    {
        if (animatorPlaying != nullptr && animatorPlaying == playerAnimators.GetAnimator(
            attackController.GetCurrState()))
        {
            animatorPlaying->Pause();
        }
        return;
    }

    if (!attackController.IsAttacking())
    {
        // if state not changed
        if (animatorPlaying != nullptr && animatorPlaying == playerAnimators.GetAnimator(
            normalStateMachine.GetCurrState()))
        {
            animatorPlaying->Update(deltaTime);
            return;
        }

        if (animatorPlaying != nullptr)
        {
            animatorPlaying->Stop();
        }

        animatorPlaying = playerAnimators.GetAnimator(normalStateMachine.GetCurrState());
        if (animatorPlaying == nullptr)
        {
            animatorPlaying = playerAnimators.GetAnimator(PlayerNormalState::IDLE);
        }
        animatorPlaying->Play();
        animatorPlaying->Update(deltaTime);
    }

    if (attackController.IsAttacking())
    {
        if (animatorPlaying != nullptr && animatorPlaying == playerAnimators.GetAnimator(
            attackController.GetCurrState()))
        {
            animatorPlaying->Update(deltaTime);
            return;
        }

        if (animatorPlaying != nullptr)
        {
            animatorPlaying->Stop();
        }

        animatorPlaying = playerAnimators.GetAnimator(attackController.GetCurrState());
        if (animatorPlaying == nullptr)
        {
            animatorPlaying = playerAnimators.GetAnimator(PlayerCombatState::ATTK1);
        }
        animatorPlaying->Play(false);
        animatorPlaying->Update(deltaTime);
    }
}

void PlayerController::DrawTrail(const Camera& cam) const
{
    if (isDrawTrail || trailFadeTimer > 0)
    {
        constexpr int trailCount = 3;
        constexpr int framesBetween = 1;

        const float fadeMult = isDashing ? 1.0f : trailFadeTimer / trailFadeDuration;

        for (int i = 1; i <= trailCount; i++)
        {
            PlayerSnapshot trail;

            if (!TimeManager::Instance().GetSnapshotAt(i * framesBetween, trail))
            {
                continue;
            }

            if (trail.frame == nullptr)
            {
                continue;
            }

            float t = 1.0f - (static_cast<float>(i) / trailCount);
            BYTE brightness = static_cast<BYTE>(200 * t * fadeMult);
            DebugPrintf("brightness = %d \n", brightness);
            RGBQUAD trailColor = {brightness, static_cast<BYTE>(brightness * 0.5), 255, 0};

            DrawTrailFromSnapshot(cam, trail, trailColor, fadeMult);
        }
    }
}

void PlayerController::DrawRewind(const Camera& cam) const
{
    // Draw rewind path preview
    constexpr int framesBetween = 3;
    const int previewCount = TimeManager::Instance().GetTimeRewindMagnitude() * 30 * framesBetween;

    for (int i = 1; i <= previewCount; i++)
    {
        PlayerSnapshot preview;
        int readPos = i * framesBetween; // curr rewind index

        if (!TimeManager::Instance().GetSnapshotAt(readPos, preview))
        {
            continue;
        }
        if (preview.frame == nullptr)
        {
            continue;
        }

        bool isAttackFrame = TimeManager::Instance().HasAttackFrame() &&
            std::abs(readPos - TimeManager::Instance().GetBestAttackFrame().snapshotIndex) <= 1;

        float t = 1 - static_cast<float>(i) / static_cast<float>(previewCount);
        BYTE brightness;
        RGBQUAD color;

        if (isAttackFrame)
        {
            color = {20, 20, 255, 0};
            DrawTrailFromSnapshot(cam, preview, color, 1);
        }
        else
        {
            brightness = static_cast<BYTE>(150 * t);
            color = {brightness, static_cast<BYTE>(static_cast<float>(brightness) * 0.8f), static_cast<BYTE>(static_cast<float>(brightness) * 0.3f), 0};
            DrawTrailFromSnapshot(cam, preview, color, t);
        }
    }
}

void PlayerController::DrawTrailFromSnapshot(const Camera& cam, const PlayerSnapshot& snapshotToDraw,
                                             const RGBQUAD& trailColor, const float fadeMult) const
{
    if (snapshotToDraw.frame == nullptr)
    {
        return;
    }

    const int screenX = cam.WorldToScreenX(snapshotToDraw.transform.topLeft.x);
    const int screenY = cam.WorldToScreenY(snapshotToDraw.transform.topLeft.y);

    if (!cam.IsVisible(snapshotToDraw.transform.topLeft.x, snapshotToDraw.transform.topLeft.y,
                       static_cast<float>(snapshotToDraw.frame->width), static_cast<float>(snapshotToDraw.frame->height)))
    {
        return;
    }

    auto buffer = reinterpret_cast<unsigned char*>(snapshotToDraw.frame->pixel);
    for (int py = 0; py < snapshotToDraw.frame->height; py++)
    {
        for (int px = 0; px < snapshotToDraw.frame->width; px++)
        {
            if (*buffer != 0)
            {
                int drawX = !snapshotToDraw.isFacingRight
                                ? (snapshotToDraw.frame->width - px - 1) + screenX
                                : px + screenX;
                DrawPixelDithered(drawX, py + screenY, trailColor, fadeMult);
            }
            buffer++;
        }
    }
}
