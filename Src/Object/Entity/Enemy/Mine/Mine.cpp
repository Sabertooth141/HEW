//
// Created by saber on 2/3/2026.
//

#include "Mine.h"
#include "../../../../Lib/conioex_custom.h"
#include "../../../../VFX/AttackVFXManager.h"

void Mine::Start()
{
    Enemy::Start();

    SetTimeToExplode();
}

void Mine::Initialize(const MineConfig& config)
{
    Enemy::Initialize(config);

    timeToExplode = config.timeToExplode;
    explosionRadius = config.explosionRadius;

    canExplode = true;
}

void Mine::Update(const float deltaTime, const float trueDeltaTime, Tilemap& tileMap)
{
    Enemy::Update(deltaTime, trueDeltaTime, tileMap);

    if (attackCooldownTimer > 0)
    {
        attackCooldownTimer -= deltaTime;
    }
    else
    {
        attackCooldownTimer = 0;
        canAttack = true;
    }

    HandleExplosion(deltaTime);
}

void Mine::Draw(Camera& cam)
{
    if (isExploding)
    {
        const float screenX = cam.WorldToScreenX(transform.center.x);
        const float screenY = cam.WorldToScreenY(transform.center.y);
        DrawCircleDithered(screenX, screenY, currIndicatorRadius, RED, true, 0.1);
        DrawCircle(screenX, screenY, explosionRadius, RED, false);
    }

    Enemy::Draw(cam);
}

void Mine::HandleMovement(const float deltaTime, Tilemap& tileMap)
{
    Enemy::HandleMovement(deltaTime, tileMap);
}

void Mine::HandleAttack(Entity* inTarget)
{
    if (!canAttack)
    {
        return;
    }

    Enemy::HandleAttack(inTarget);

    isExploding = true;
}

bool Mine::CanStartAttack(const float deltaTime)
{
    if (CheckCollision(&target->transform) && canExplode)
    {
        return true;
    }
    return false;
}

void Mine::HandleExplosion(const float deltaTime)
{
    if (isExploding)
    {
        elapsedTime += deltaTime;
        float t = elapsedTime / timeToExplode;
        currIndicatorRadius = Lerp(0, explosionRadius, t);
    }

    if (currIndicatorRadius >= explosionRadius)
    {
        AttackVFXManager::Instance().PlayAttackVFX(&transform, Vector2(0, 0), EnemyVFXType::MINE, false);
        if (CheckCircleRectCollision(transform.center.x, transform.center.y, explosionRadius, &target->transform))
        {
            target->TakeDamage(damage);
        }

        currIndicatorRadius = 0;
        elapsedTime = 0;
        isExploding = false;
        stateMachine.ChangeState(EnemyState::IDLE);
        canAttack = false;
        attackCooldownTimer = attackCooldown;
    }
}

void Mine::SetTimeToExplode()
{
    SpriteSheet* attkSheet = animators.GetAnimator(EnemyState::ATTK)->GetSpriteSheet();
    float duration = 0;

    for (const auto frame : attkSheet->frames)
    {
        duration += frame.duration;
    }

    timeToExplode = duration;
}
