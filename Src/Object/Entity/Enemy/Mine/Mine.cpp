//
// Created by saber on 2/3/2026.
//

#include "Mine.h"
#include "../../../../Lib/conioex_custom.h"

void Mine::Initialize(const MineConfig& config)
{
    Enemy::Initialize(config);

    timeToExplode = config.timeToExplode;
    explosionRadius = config.explosionRadius;

    canExplode = true;
}

void Mine::Update(const float deltaTime, const Tilemap& tileMap)
{
    Enemy::Update(deltaTime, tileMap);

    if (CheckCollision(targetTransform) && canExplode)
    {
        isExploding = true;
    }

    HandleExplosion(deltaTime);
}

void Mine::Draw(const Camera& cam)
{
    Enemy::Draw(cam);

    if (isExploding)
    {
        const float screenX = cam.WorldToScreenX(transform.center.x);
        const float screenY = cam.WorldToScreenY(transform.center.y);
        DrawCircleDithered(screenX, screenY, currIndicatorRadius, RED, true, 0.7);
        DrawCircle(screenX, screenY, explosionRadius, RED, false);
    }
}

void Mine::HandleMovement(const float deltaTime, const Tilemap& tileMap)
{
    currSpeedX = stateMachine.GetCurrentState() == EnemyState::ATTK ? currSpeedX = moveSpeed * 0.2f : currSpeedX;

    Enemy::HandleMovement(deltaTime, tileMap);
}

void Mine::HandleAttack(const Vector2 targetPos)
{
    Enemy::HandleAttack(targetPos);
}

void Mine::HandleExplosion(const float deltaTime)
{
    if (deltaTime == 0)
    {
        return;
    }

    if (isExploding)
    {
        elapsedTime += deltaTime;
        float t = elapsedTime / timeToExplode;
        currIndicatorRadius = Lerp(0, explosionRadius, t);
    }

    if (currIndicatorRadius >= explosionRadius)
    {
        currIndicatorRadius = 0;
        elapsedTime = 0;
        isExploding = false;
    }
}

