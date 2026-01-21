//
// Created by saber on 1/14/2026.
//

#include "PlayerController.h"

#include "../../../Systems/TimeManager.h"

PlayerController::PlayerController() : walkSpeed(0), sprintSpeed(0), jumpForce(0), timeStopDuration(0)
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


}

void PlayerController::Draw(const Camera& cam)
{
    Entity::Draw(cam);
}

void PlayerController::Die()
{
    Entity::Die();
}

void PlayerController::HandleInput()
{
    if (input.moveLeft.IsPressed())
    {
        velX = -currSpeed;
    }
    else if (input.moveRight.IsPressed())
    {
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
}


