//
// Created by saber on 2/10/2026.
//

#include "UGV.h"

#include "../../../../Lib/conioex_custom.h"

void UGV::Initialize(const UGVConfig& config)
{
    Enemy::Initialize(config);

    attackWindUpDuration = config.attackWindupDuration;
    knockBackForce = config.knockBackForce;
}

void UGV::Update(const float deltaTime, Tilemap& tileMap)
{
    Enemy::Update(deltaTime, tileMap);

    if (attackWindUpTimer > 0)
    {
        attackWindUpTimer -= deltaTime;
    }
    else if (isAttacking && attackWindUpTimer <= 0)
    {
        attackWindUpTimer = 0;
        HandleAttackKnockBack();
    }

    if (attackCooldownTimer > 0)
    {
        attackCooldownTimer -= deltaTime;
        canAttack = false;
    }
    else if (!canAttack && attackCooldownTimer <= 0)
    {
        if (stateMachine.GetCurrentState() == EnemyState::ATTK)
        {
            stateMachine.ChangeState(EnemyState::IDLE);
        }
        attackCooldownTimer = 0;
        canAttack = true;
    }
}

void UGV::HandleAttack(Entity* inTarget)
{
    if (stateMachine.GetCurrentState() == EnemyState::HURT)
    {
        return;
    }

    if (stateMachine.GetCurrentState() == EnemyState::ATTK)
    {
        return;
    }

    HandleAttackWindUp();
}

void UGV::HandleAnimationUpdate(const float deltaTime)
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

    if (stateMachine.GetCurrentState() == EnemyState::ATTK)
    {
        animatorPlaying->Play(false, true);
        return;
    }

    animatorPlaying->Play();
    animatorPlaying->Update(deltaTime);
}

void UGV::HandleAttackWindUp()
{
    if (isAttacking)
    {
        return;
    }

    if (std::abs(target->transform.center.y - transform.center.y) > 20)
    {
        stateMachine.ChangeState(EnemyState::IDLE);
        return;
    }

    if (std::abs(target->transform.center.x - transform.center.x) > attackDistance)
    {
        stateMachine.ChangeState(EnemyState::IDLE);
        return;
    }

    attackWindUpTimer = attackWindUpDuration;
    isAttacking = true;
}

void UGV::HandleAttackKnockBack()
{
    stateMachine.ChangeState(EnemyState::ATTK);

    if (!transform.CheckDistance(target->transform.center, attackDistance))
    {
        isAttacking = false;
        canAttack = false;
        attackCooldownTimer = attackCooldown;
        return;
    }

    if (std::abs(target->transform.center.y - transform.center.y) > attackDistance)
    {
        isAttacking = false;
        canAttack = false;
        attackCooldownTimer = attackCooldown;
        return;
    }

    if (target->transform.center.x - transform.center.x > 0)
    {
        target->TakeKnockback(knockBackForce, true);
    }
    else
    {
        target->TakeKnockback(knockBackForce, false);
    }
    target->TakeDamage(damage);

    attackCooldownTimer = attackCooldown;
    isAttacking = false;
    canAttack = false;
}
