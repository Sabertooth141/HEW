//
// Created by saber on 1/14/2026.
//

#include "PlayerController.h"

#include "../../../Lib/Shape.h"
#include "../../../Systems/TimeManager.h"

PlayerController::PlayerController() : walkSpeed(0), sprintSpeed(0), jumpForce(0), timeStopDuration(0), snapshot()
{
}

void PlayerController::Initialize(const PlayerConfig& config)
{
    Entity::Initialize(config);

    walkSpeed = config.walkSpeed;
    sprintSpeed = config.sprintSpeed;
    jumpForce = config.jumpForce;
    timeStopDuration = config.timeStopDuration;
}

void PlayerController::Start()
{
    Entity::Start();

    currSpeed = walkSpeed;
}

void PlayerController::Update(const float deltaTime, const Tilemap& tileMap)
{
    HandleInput();

    Entity::Update(deltaTime, tileMap);

    snapshot.x = x;
    snapshot.y = y;
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
    DrawRect(screenX, screenY, screenX + width, screenY + height, RED, false);

    if (attackHitbox.isActive)
    {
        screenX = cam.WorldToScreenX(attackHitbox.x);
        screenY = cam.WorldToScreenY(attackHitbox.y);
        DrawRect(screenX, screenY, screenX + attackHitbox.width, screenY + attackHitbox.height, LIGHTRED, false);
    }
}

void PlayerController::Die()
{
    Entity::Die();
}

void PlayerController::HandleInput()
{
    if (input.moveLeft.IsPressed())
    {
        isFacingRight = false;
        velX = -currSpeed;
    }
    else if (input.moveRight.IsPressed())
    {
        isFacingRight = true;
        velX = currSpeed;
    }
    else
    {
        velX = 0;
    }

    if (input.jump.IsEdge() && isGrounded)
    {
        velY = -jumpForce;
    }

    if (input.attack.IsEdge())
    {
        Attack();
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

    x = playerSnapshot.x;
    y = playerSnapshot.y;
    velX = playerSnapshot.velX;
    velY = playerSnapshot.velY;
    isFacingRight = playerSnapshot.isFacingRight;
}
