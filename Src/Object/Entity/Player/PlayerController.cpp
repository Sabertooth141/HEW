//
// Created by saber on 1/14/2026.
//

#include "PlayerController.h"

#include "../../../Animation/Animator.h"
#include "../../../Animation/SpriteSheetLoader.h"
#include "../../../Lib/Shape.h"
#include "../../../Systems/TimeManager.h"

PlayerController::PlayerController() : walkSpeed(0), sprintSpeed(0),
                                       jumpForce(0),
                                       timeStopDuration(0),
                                       airResistance(0),
                                       snapshot(),
                                       moveStateMachine(PlayerMoveState::DEFAULT)
{
}

void PlayerController::Initialize(const PlayerConfig& config, const PlayerAttackConfig& attackConfig)
{
    Entity::Initialize(config);

    walkSpeed = config.walkSpeed;
    sprintSpeed = config.sprintSpeed;
    jumpForce = config.jumpForce;
    timeStopDuration = config.timeStopDuration;
    airResistance = config.airResistance;

    attackController.Initialize(attackConfig);
    InitAnimations();
}

void PlayerController::Start()
{
    Entity::Start();

    currSpeed = walkSpeed;
    moveStateMachine.ChangeState(PlayerMoveState::IDLE);
}

void PlayerController::Update(const float deltaTime, const Tilemap& tileMap)
{
    HandleInput(deltaTime);
    attackController.Update(deltaTime, transform, isFacingRight);

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
    Entity::Draw(cam);

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

void PlayerController::HandleMovement(const float deltaTime, const Tilemap& tileMap)
{
    if (!attackController.CanMove())
    {
        return;
    }

    Entity::HandleMovement(deltaTime, tileMap);
}

void PlayerController::ApplyPhysics(const float deltaTime)
{
    if (!attackController.CanMove())
    {
        return;
    }

    Entity::ApplyPhysics(deltaTime);
}

void PlayerController::HandleInput(const float deltaTime)
{
    if (input.moveLeft.IsPressed())
    {
        isFacingRight = false;
        velX = -currSpeed;
        moveStateMachine.ChangeState(PlayerMoveState::MOVE);
    }
    else if (input.moveRight.IsPressed())
    {
        isFacingRight = true;
        velX = currSpeed;
        moveStateMachine.ChangeState(PlayerMoveState::MOVE);
    }
    else
    {
        if (isGrounded)
        {
            moveStateMachine.ChangeState(PlayerMoveState::IDLE);
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
        velY = -jumpForce;
        moveStateMachine.ChangeState(PlayerMoveState::JUMP);
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

void PlayerController::InitAnimations()
{
    const SpriteSheet* moveSprite = SpriteSheetLoader::LoadFromFile("../Assets/Player/PlayerMove/PlayerMove.bmp",
                                                              "../Assets/Player/PlayerMove/PlayerMove.json");
    animations.AddAnimation(PlayerAnimations::MOVE, *moveSprite);
}
