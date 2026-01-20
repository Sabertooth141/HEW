//
// Created by saber on 1/14/2026.
//

#include "PlayerController.h"

PlayerController::PlayerController(const PlayerConfig& config) : Entity(config),
                                                                 sprintSpeed(config.sprintSpeed)
{
}

void PlayerController::Start(float x, float y)
{
}

void PlayerController::Update(float deltaTime, const Tilemap& tileMap)
{
}

void PlayerController::Draw(const Camera& cam)
{
}

void PlayerController::HandleMovement()
{
}

void PlayerController::Die()
{
}
