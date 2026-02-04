//
// Created by saber on 1/22/2026.
//

#include "Enemy.h"

#include "../../../Systems/EnemyManager.h"

Enemy::Enemy() : targetTransform(nullptr), attackCooldown(0), moveSpeed(0), damage(0), stateMachine(EnemyState::DEFAULT)
{
}

void Enemy::Initialize(const EnemyConfig& config)
{
    Entity::Initialize(config);

    targetTransform = config.targetTransform;
    attackCooldown = config.attackCooldown;
    moveSpeed = config.moveSpeed;
}

void Enemy::Start()
{
    Entity::Start();

    stateMachine.ChangeState(EnemyState::IDLE);
    currSpeedX = moveSpeed;
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
    velX = currSpeedX;

    const float newX = transform.topLeft.x + velX * deltaTime;

    if (CheckCollisionX(tilemap, newX))
    {
        currSpeedX = -currSpeedX;
    }
}

void Enemy::HandleAttack(Vector2 targetPos)
{
}

void Enemy::Die()
{
    Entity::Die();

    EnemyManager::Instance().UnregisterEnemy(this);
}

void Enemy::TargetDetection()
{
}
