//
// Created by saber on 1/14/2026.
//

#include "PlayerController.h"

#include <memory>

#include "../../../Core/Game.h"
#include "../../../Systems/TimeManager.h"

PlayerController::PlayerController() : walkSpeed(0), normalDashSpeed(0), currDashSpeed(0),
                                       jumpForce(0),
                                       timeStopDuration(0),
                                       airResistance(0), normalGravity(0), isDamageable(true), dashTimer(0),
                                       normalDashDuration(0),
                                       currDashDuration(0),
                                       isDashing(false),
                                       snapshot(),
                                       normalStateMachine(PlayerNormalState::DEFAULT), attackController(),
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

void PlayerController::Update(const float deltaTime, const Tilemap& tileMap)
{
    HandleInput(deltaTime);
    attackController.Update(deltaTime, transform, isFacingRight);
    HandleAnimationUpdate(deltaTime);

    Entity::Update(deltaTime, tileMap);

    snapshot.x = transform.topLeft.x;
    snapshot.y = transform.topLeft.y;
    snapshot.velX = velX;
    snapshot.velY = velY;
    snapshot.isFacingRight = isFacingRight;

    TimeManager::Instance().RecordPlayerSnapshot(snapshot);
}

void PlayerController::Draw(const Camera& cam)
{
    if (animatorPlaying != nullptr)
    {
        animatorPlaying->Draw(cam, transform.topLeft.x, transform.topLeft.y, !isFacingRight);
    }

    if (!cam.IsVisible(transform.topLeft.x, transform.topLeft.y, transform.size.x, transform.size.y))
    {
        return;
    }

    const PlayerSnapshot holoSnapshot = TimeManager::Instance().GetPlayerSnapshot();
    int screenX = cam.WorldToScreenX(holoSnapshot.x);
    int screenY = cam.WorldToScreenY(holoSnapshot.y);
    DrawRect(screenX, screenY, screenX + transform.size.x, screenY + transform.size.y, RED, false);

    Hitbox& hitbox = attackController.GetHitBox();
    if (hitbox.isActive)
    {
        screenX = cam.WorldToScreenX(hitbox.transform.topLeft.x);
        screenY = cam.WorldToScreenY(hitbox.transform.topLeft.y);

        DrawRect(screenX, screenY, screenX + hitbox.transform.size.x, screenY + hitbox.transform.size.y, LIGHTRED,
                 false);
    }
}

void PlayerController::Die()
{
    Entity::Die();
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
}

SpriteSheet* PlayerController::GetSpriteSheetFromAnimator(const PlayerCombatState attkState)
{
    const Animator* resAnimator = playerAnimators.GetAnimator(attkState);
    return resAnimator->GetSpriteSheet();
}

void PlayerController::HandleMovement(const float deltaTime, const Tilemap& tileMap)
{
    if (!isDashing)
    {
        currSpeedX = !attackController.CanMove() ? SPEED_WHEN_ATTK : walkSpeed;
    }
    else
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

void PlayerController::HandleInput(const float deltaTime)
{
    if (input.moveLeft.IsPressed())
    {
        if (isDashing)
        {
            return;
        }

        if (attackController.CanMove())
        {
            isFacingRight = false;
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

        if (attackController.CanMove())
        {
            isFacingRight = true;
        }

        velX = currSpeedX;
        normalStateMachine.ChangeState(PlayerNormalState::MOVE);
    }
    else
    {
        if (isGrounded && !isDashing)
        {
            normalStateMachine.ChangeState(PlayerNormalState::IDLE);
            velX = 0;
        }
        else
        {
            velX = velX + (0 - velX) * airResistance * deltaTime;

            if (fabs(velX) < 0.1f)
            {
                velX = 0;
            }
        }
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
    PlayerSnapshot playerSnapshot{};
    if (!TimeManager::Instance().GetPlayerSnapshot(playerSnapshot))
    {
        return;
    }

    transform.topLeft.x = playerSnapshot.x;
    transform.topLeft.y = playerSnapshot.y;
    velX = playerSnapshot.velX;
    velY = playerSnapshot.velY;
    isFacingRight = playerSnapshot.isFacingRight;
}

void PlayerController::HandleAnimationUpdate(const float deltaTime)
{
    if (attackController.IsInRecovery())
    {
        OutputDebugStringA("RECOVERY\n");
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
