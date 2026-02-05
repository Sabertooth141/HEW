//
// Created by saber on 1/22/2026.
//

#include "Enemy.h"

#include "../../../Lib/conioex_custom.h"
#include "../../../Systems/EnemyManager.h"

Enemy::Enemy() : target(nullptr), attackCooldown(0), moveSpeed(0), damage(0), invicCD(0),
                 invicTimer(0),
                 stateMachine(EnemyState::DEFAULT)
{
}

void Enemy::Initialize(const EnemyConfig& config)
{
    Entity::Initialize(config);

    target = config.target;
    attackCooldown = config.attackCooldown;
    moveSpeed = config.moveSpeed;
    invicCD = config.invicCooldown;
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

    HandleAnimationUpdate(deltaTime);

    if (DetectTarget(deltaTime))
    {
        HandleAttack(target);
    }

    if (stateMachine.GetCurrentState() == EnemyState::HURT)
    {
        color = YELLOW;
        invicTimer += deltaTime;

        if (invicTimer >= invicCD)
        {
            color = GREEN;
            invicTimer = 0;
            stateMachine.ChangeState(stateMachine.GetPreviousState());
        }
    }
}

void Enemy::Draw(const Camera& cam)
{
    if (!cam.IsVisible(transform.topLeft.x, transform.topLeft.y, transform.size.x, transform.size.y))
    {
        return;
    }

    if (animatorPlaying != nullptr)
    {
        animatorPlaying->Draw(cam, transform.topLeft.x, transform.topLeft.y, !isFacingRight);
    }
}

void Enemy::HandleMovement(const float deltaTime, const Tilemap& tilemap)
{
    if (stateMachine.GetCurrentState() == EnemyState::ATTK)
    {
        return;
    }

    if (stateMachine.GetCurrentState() == EnemyState::HURT)
    {
        return;
    }

    Entity::HandleMovement(deltaTime, tilemap);

    // FOR TESTING PURPOSES
    stateMachine.ChangeState(EnemyState::MOVE);
    velX = currSpeedX;

    const float newX = transform.topLeft.x + velX * deltaTime;

    if (CheckCollisionX(tilemap, newX))
    {
        currSpeedX = -currSpeedX;
    }
}

void Enemy::TakeDamage(const float inDamage)
{
    if (stateMachine.GetCurrentState() == EnemyState::HURT)
    {
        return;
    }

    Entity::TakeDamage(inDamage);

    stateMachine.ChangeState(EnemyState::HURT);
}

void Enemy::InitAnimation(const EnemyAnimPaths& path)
{
    std::unique_ptr<Animator> animator = std::make_unique<Animator>();
    animator->LoadSpriteSheet(path.jsonPath.c_str(), path.bmpPath.c_str());
    animator->SetLoopStartFrame(path.startFrame);

    animators.AddAnimator(path.animationState, std::move(animator));
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
        animatorPlaying = animators.GetAnimator(EnemyState::MOVE);
    }
    animatorPlaying->Play();
    animatorPlaying->Update(deltaTime);
}

void Enemy::Die()
{
    Entity::Die();

    EnemyManager::Instance().UnregisterEnemy(this);
}

bool Enemy::DetectTarget(const float deltaTime)
{
    return false;
}
