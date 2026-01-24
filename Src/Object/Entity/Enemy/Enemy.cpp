//
// Created by saber on 1/22/2026.
//

#include "Enemy.h"

#include "../../../Systems/EnemyManager.h"

Enemy::Enemy() : target({}), attackCooldown(0), moveSpeed(0)
{
}

void Enemy::Initialize(const EnemyConfig& config)
{
    Entity::Initialize(config);

    target = config.target;
    attackCooldown = config.attackCooldown;
    moveSpeed = config.moveSpeed;
}

void Enemy::Start()
{
    Entity::Start();

    currSpeed = moveSpeed;
}

void Enemy::Update(const float deltaTime, const Tilemap& tileMap)
{
    Entity::Update(deltaTime, tileMap);
}

void Enemy::Draw(const Camera& cam)
{
    Entity::Draw(cam);
}

void Enemy::HandleMovement(const float deltaTime, const Tilemap& tilemap)
{
    Entity::HandleMovement(deltaTime, tilemap);

    // FOR TESTING PURPOSES
    velX = currSpeed;

    const float newX = x + velX * deltaTime;

    if (CheckCollisionX(tilemap, newX))
    {
        currSpeed = -currSpeed;
    }
}

void Enemy::Die()
{
    Entity::Die();

    EnemyManager::Instance().UnregisterEnemy(this);
}

void Enemy::TargetDetection()
{
}
