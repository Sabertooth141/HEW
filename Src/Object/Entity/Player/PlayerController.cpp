//
// Created by saber on 1/14/2026.
//

#include "PlayerController.h"

#include <memory>

#include "../../../Game/Scenes/GameScene.h"
#include "../../../Systems/TimeManager.h"
#include "../../../Lib/conioex_custom.h"
#include "../../../Systems/GameManager.h"

PlayerController::PlayerController() : walkSpeed(0), normalDashSpeed(0), currDashSpeed(0),
                                       jumpForce(0),
                                       timeStopDuration(0),
                                       airResistance(0), normalGravity(0), isDamageable(true), dashTimer(0),
                                       normalDashDuration(0),
                                       currDashDuration(0),
                                       isDashing(false),
                                       snapshot(),
                                       normalStateMachine(PlayerNormalState::DEFAULT),
                                       animatorPlaying(nullptr)
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

void PlayerController::Update(const float deltaTime, Tilemap& tileMap)
{
    if (TimeManager::Instance().IsRewinding())
    {
        PlayerSnapshot rewind;
        if (TimeManager::Instance().UpdateRewind(deltaTime, rewind))
        {
            transform.topLeft.x = rewind.x;
            transform.topLeft.y = rewind.y;
            transform.CalculateCenterPosition();
            velX = rewind.velX;
            velY = rewind.velY;
            isFacingRight = rewind.isFacingRight;
            rewindSnapshot = rewind;
        }

        return;
    }

    HandleInput(deltaTime);
    attackController.Update(deltaTime, transform, isFacingRight);
    HandleAnimationUpdate(deltaTime);

    Entity::Update(deltaTime, tileMap);

    if (!isKnockedBack && normalStateMachine.GetCurrState() == PlayerNormalState::KNOCKBACK)
    {
        normalStateMachine.ChangeState(PlayerNormalState::IDLE);
    }

    if (trailFadeTimer > 0)
    {
        trailFadeTimer -= deltaTime;
    }

    snapshot.x = transform.topLeft.x;
    snapshot.y = transform.topLeft.y;
    snapshot.velX = velX;
    snapshot.velY = velY;
    snapshot.isFacingRight = isFacingRight;
    snapshot.frame = animatorPlaying ? animatorPlaying->GetCurrentFrameBmp() : nullptr;

    TimeManager::Instance().RecordPlayerSnapshot(snapshot);
}

void PlayerController::Draw(Camera& cam)
{
    DrawTrail(cam);

    if (TimeManager::Instance().IsRewinding())
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
        animatorPlaying->Draw(cam, transform.topLeft.x, transform.topLeft.y, !isFacingRight);
    }

    // const PlayerSnapshot holoSnapshot = TimeManager::Instance().GetPlayerSnapshot();
    // int screenX = cam.WorldToScreenX(holoSnapshot.x);
    // int screenY = cam.WorldToScreenY(holoSnapshot.y);
    // DrawRect(screenX, screenY, screenX + transform.size.x, screenY + transform.size.y, RED, false);

    Hitbox& hitbox = attackController.GetHitBox();
    if (hitbox.isActive)
    {
        int screenX = cam.WorldToScreenX(hitbox.transform.topLeft.x);
        int screenY = cam.WorldToScreenY(hitbox.transform.topLeft.y);

        DrawRect(screenX, screenY, screenX + hitbox.transform.size.x, screenY + hitbox.transform.size.y, LIGHTRED,
                 false);
    }
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

SpriteSheet* PlayerController::GetSpriteSheetFromAnimator(const PlayerCombatState attkState)
{
    const Animator* resAnimator = playerAnimators.GetAnimator(attkState);
    return resAnimator->GetSpriteSheet();
}

void PlayerController::HandleMovement(const float deltaTime, Tilemap& tileMap)
{
    if (isGrounded && !isDashing && !isKnockedBack)
    {
        if (!isMovingInput)
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
        if (!attackController.CanMove())
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
            isDamageable = true;
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
        else
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
        else
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
        velY = -jumpForce;
        normalStateMachine.ChangeState(PlayerNormalState::JUMP);
    }

    if (input.dash.IsEdge())
    {
        if (attackController.IsAttacking())
        {
            return;
        }

        if (attackController.IsInRecovery())
        {
            attackController.CancelCombo();
        }
        Dash(normalDashSpeed, normalDashDuration, true);
        normalStateMachine.ChangeState(PlayerNormalState::MOVE);
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
        HandleTimeRewind();
    }
}

void PlayerController::HandleTimeRewind()
{
    TimeManager::Instance().StartRewind();
    // PlayerSnapshot playerSnapshot{};
    // if (!TimeManager::Instance().GetPlayerSnapshot(playerSnapshot))
    // {
    //     return;
    // }
    //
    // transform.topLeft.x = playerSnapshot.x;
    // transform.topLeft.y = playerSnapshot.y;
    // velX = playerSnapshot.velX;
    // velY = playerSnapshot.velY;
    // isFacingRight = playerSnapshot.isFacingRight;
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
    if (TimeManager::Instance().IsRewinding())
    {
        constexpr int previewCount = 30;
        constexpr int framesBetween = 4;

        for (int i = 1; i <= previewCount; i++)
        {
            PlayerSnapshot preview;
            int readPos = /* current rewindReadHead */ i * framesBetween;

            if (!TimeManager::Instance().GetSnapshotAt(readPos, preview))
                continue;
            if (preview.frame == nullptr)
                continue;
            float t = (static_cast<float>(i) / (previewCount / 3));
            BYTE brightness = static_cast<BYTE>(150 * t);
            RGBQUAD color = {brightness, static_cast<BYTE>(brightness * 0.8f), static_cast<BYTE>(brightness * 0.3f), 0};

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

    const int screenX = cam.WorldToScreenX(snapshotToDraw.x);
    const int screenY = cam.WorldToScreenY(snapshotToDraw.y);

    if (!cam.IsVisible(snapshotToDraw.x, snapshotToDraw.y, snapshotToDraw.frame->width, snapshotToDraw.frame->height))
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
