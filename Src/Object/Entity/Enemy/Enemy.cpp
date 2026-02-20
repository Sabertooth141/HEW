//
// Created by saber on 1/22/2026.
//

#include "Enemy.h"

#include "../../../Lib/conioex_custom.h"
#include "../../../Systems/EnemyManager.h"
#include "../../../Systems/TimeManager.h"
#include "../../../VFX/AttackVFXManager.h"

Enemy::Enemy() : target(nullptr), moveSpeed(0), damage(0), canAttack(true), attackCooldown(0),
                 attackCooldownTimer(0), isAttacking(false),
                 attackDistance(0),
                 detectionDistance(0), isAlive(true),
                 stateMachine(EnemyState::DEFAULT), spawnGroupID(0)
{
}

void Enemy::Initialize(const EnemyConfig& config)
{
    Entity::Initialize(config);

    target = config.target;
    attackCooldown = config.attackCooldown;
    moveSpeed = config.moveSpeed;
    attackDistance = config.attackDistance;
    detectionDistance = config.detectionDistance;
    damage = config.damage;
    spawnGroupID = config.spawnGroupID;
}

void Enemy::Start()
{
    Entity::Start();

    stateMachine.ChangeState(EnemyState::IDLE);
    currSpeedX = moveSpeed;
    isAlive = true;
}

void Enemy::Update(const float deltaTime, const float trueDeltaTime, Tilemap& tileMap)
{
    Entity::Update(deltaTime, trueDeltaTime, tileMap);

    HandleAnimationUpdate(deltaTime);

    if (CanStartAttack(deltaTime))
    {
        HandleAttack(target);
    }

    if (stateMachine.GetCurrentState() != EnemyState::ATTK)
    {
        if (!canAttack || isAttacking)
        {
            stateMachine.ChangeState(EnemyState::IDLE);
            return;
        }

        if (CanStartPathfinding(deltaTime))
        {
            stateMachine.ChangeState(EnemyState::PATHFIND);
        }
        else
        {
            stateMachine.ChangeState(EnemyState::PATROL);
        }
    }

    if (velX > 0.f)
    {
        isFacingRight = true;
    }
    else if (velX < 0.f)
    {
        isFacingRight = false;
    }
}

void Enemy::Draw(Camera& cam)
{
    if (!cam.IsVisible(transform.topLeft.x, transform.topLeft.y, transform.size.x, transform.size.y))
    {
        return;
    }

    if (animatorPlaying != nullptr)
    {
        animatorPlaying->Draw(cam, transform.topLeft.x, transform.topLeft.y, isFacingRight);
    }
}

void Enemy::HandleMovement(const float deltaTime, Tilemap& tilemap)
{
    if (stateMachine.GetCurrentState() != EnemyState::PATROL &&
        stateMachine.GetCurrentState() != EnemyState::PATHFIND &&
        stateMachine.GetCurrentState() != EnemyState::IDLE)
    {
        return;
    }

    if (isAttacking)
    {
        velX = 0;
        return;
    }

    if (stateMachine.GetCurrentState() != EnemyState::ATTK &&
        stateMachine.GetCurrentState() != EnemyState::HURT)
    {
        if (velX > 0.f)
        {
            isFacingRight = true;
        }
        else if (velX < 0.f)
        {
            isFacingRight = false;
        }
    }

    if (stateMachine.GetCurrentState() == EnemyState::PATROL)
    {
        HandlePatrol(tilemap, deltaTime);
    }

    if (stateMachine.GetCurrentState() == EnemyState::PATHFIND)
    {
        PathfindToTarget(deltaTime);
    }

    Entity::HandleMovement(deltaTime, tilemap);
}

void Enemy::TakeDamage(const float inDamage)
{
    if (isInvic)
    {
        return;
    }

    AttackVFXManager::Instance().PlayAttackVFX(&transform, {0, 0}, EnemyVFXType::HIT, false);

    TimeManager::Instance().TriggerHitStop(inDamage * 0.003);
    Camera::Instance().TriggerScreenShake(1, inDamage * 0.01);

    Entity::TakeDamage(inDamage);

    stateMachine.ChangeState(EnemyState::HURT);
}

void Enemy::InitAnimation(const EnemyAnimPaths<EnemyState>& path)
{
    std::unique_ptr<Animator> animator = std::make_unique<Animator>();
    animator->LoadSpriteSheet(path.jsonPath.c_str(), path.bmpPath.c_str());
    animator->SetLoopStartFrame(path.startFrame);

    animators.AddAnimator(path.animationName, std::move(animator));
}

void Enemy::HandleAttack(Entity* inTarget)
{
    if (stateMachine.GetCurrentState() == EnemyState::HURT)
    {
        return;
    }

    if (stateMachine.GetCurrentState() == EnemyState::ATTK)
    {
        return;
    }

    stateMachine.ChangeState(EnemyState::ATTK);
}

void Enemy::HandleAnimationUpdate(const float deltaTime)
{
    if (animatorPlaying != nullptr && animatorPlaying == animators.GetAnimator(
        stateMachine.GetCurrentState()))
    {
        animatorPlaying->Update(deltaTime);
        return;
    }

    if (animatorPlaying != nullptr)
    {
        animatorPlaying->Stop();
    }

    animatorPlaying = animators.GetAnimator(stateMachine.GetCurrentState());
    if (animatorPlaying == nullptr)
    {
        animatorPlaying = animators.GetAnimator(EnemyState::PATROL);
    }
    animatorPlaying->Play();
    animatorPlaying->Update(deltaTime);
}

void Enemy::Die()
{
    Entity::Die();

    isAlive = false;
    EnemyManager::Instance().UnregisterEnemy(this);
}

bool Enemy::CanStartPathfinding(float deltaTime)
{
    if (target == nullptr)
    {
        return false;
    }

    return transform.CheckDistance(target->transform.center, detectionDistance);
}

bool Enemy::CanStartAttack(float deltaTime)
{
    if (target == nullptr)
    {
        return false;
    }

    if (!canAttack)
    {
        return false;
    }

    return transform.CheckDistance(target->transform.center, attackDistance);
}

void Enemy::HandlePatrol(Tilemap& tilemap, const float deltaTime)
{
    velX = currSpeedX;

    const float newX = transform.topLeft.x + velX * deltaTime;

    if (CheckCollisionX(tilemap, newX) || CheckCliffAhead(tilemap))
    {
        currSpeedX = -currSpeedX;
        velX = 0;
    }
}

void Enemy::PathfindToTarget(float deltaTime)
{
    if (target == nullptr)
    {
        return;
    }

    const float targetX = target->transform.center.x;
    const float diff = targetX - transform.center.x;

    const float deadZone = attackDistance * 0.5f;

    if (std::abs(diff) < deadZone)
    {
        velX = 0;
        return;
    }

    if (targetX > transform.center.x)
    {
        velX = moveSpeed;
    }
    else
    {
        velX = -moveSpeed;
    }
}

bool Enemy::CheckCliffAhead(const Tilemap& tilemap) const
{
    float probeX;

    if (currSpeedX > 0)
    {
        probeX = transform.topLeft.x + transform.size.x + 2;
    }
    else
    {
        probeX = transform.topLeft.x - 2;
    }

    float probeY = transform.topLeft.y + transform.size.y + 1;

    int tileX = static_cast<int>(probeX) / tilemap.GetTileSize();
    int tileY = static_cast<int>(probeY) / tilemap.GetTileSize();

    if (tileX < 0 || tileX >= tilemap.GetWidthTiles() || tileY < 0 || tileY >= tilemap.GetHeightTiles())
    {
        return true;
    }

    return !tilemap.IsSolidAt(tileX, tileY);
}
