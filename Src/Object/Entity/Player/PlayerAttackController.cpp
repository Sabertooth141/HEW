//
// Created by saber on 1/28/2026.
//

#include "PlayerAttackController.h"

#include "../../../Systems/EnemyManager.h"
#include "PlayerController.h"
#include "../../../VFX/AttackVFXManager.h"

void PlayerAttackController::Initialize(const PlayerAttackConfig& config, PlayerController* controller)
{
    attackData = config.data;

    playerController = controller;
}

void PlayerAttackController::Update(const float deltaTime, Transform& playerTransform,
                                    const bool isFacingRight)
{
    HandleAttackCollisions();

    ownerTransform = &playerTransform;
    ownerFacingRight = isFacingRight;

    combatStateMachine.Update(deltaTime);
    comboTimer += deltaTime;

    const PlayerCombatState currentState = combatStateMachine.GetCurrState();

    if (currentState == PlayerCombatState::DEFAULT)
    {
        return;
    }

    const AttkData& data = GetAttackData(currentState);
    hitbox.Update(deltaTime, ownerTransform->center.x, ownerTransform->center.y);

    // attack
    if (comboTimer <= data.duration)
    {
        // char debug[64];
        // sprintf_s(debug, "CurrentAttk: %d\n", currentState);
        // OutputDebugStringA(debug);
    }
    // if combo buffered
    else if (comboTimer <= data.duration + data.recovery)
    {
        if (hitbox.isActive)
        {
            hitbox.isActive = false;
        }

        isInRecovery = true;
        if (comboInputBuffer && comboTimer >= data.duration)
        {
            comboInputBuffer = false;

            isInRecovery = false;
            AdvanceCombo(currentState, data);
        }
    }
    // complete attack
    else if (comboTimer > data.duration + data.recovery)
    {
        isInRecovery = false;
        EndAttack();
    }
}

bool PlayerAttackController::TryAttack()
{
    const PlayerCombatState currState = combatStateMachine.GetCurrState();

    // if currently not attacking - attack
    if (currState == PlayerCombatState::DEFAULT)
    {
        StartAttack(PlayerCombatState::ATTK0);
        return true;
    }

    // if currently attacking && comboable - buffer combo
    if (currState == PlayerCombatState::ATTK0 || currState == PlayerCombatState::ATTK1)
    {
        comboInputBuffer = true;
        return true;
    }

    // if not comboable
    return false;
}

void PlayerAttackController::CancelCombo()
{
    combatStateMachine.ChangeState(PlayerCombatState::DEFAULT);
    hitbox.isActive = false;
    comboInputBuffer = false;
    comboTimer = 0;
    hasHit = false;
    isInRecovery = false;
}

bool PlayerAttackController::IsAttacking() const
{
    const PlayerCombatState currState = combatStateMachine.GetCurrState();

    if (currState == PlayerCombatState::DEFAULT)
    {
        return false;
    }

    return combatStateMachine.GetCurrState() != PlayerCombatState::DEFAULT && !isInRecovery;
}

bool PlayerAttackController::CanMove() const
{
    const PlayerCombatState currState = combatStateMachine.GetCurrState();

    if (currState == PlayerCombatState::DEFAULT)
    {
        return true;
    }

    const AttkData& data = GetAttackData(currState);

    return comboTimer > data.duration + data.recovery;
}

float PlayerAttackController::GetCurrentDamage() const
{
    const PlayerCombatState currState = combatStateMachine.GetCurrState();

    if (currState == PlayerCombatState::DEFAULT)
    {
        return 0;
    }

    return GetAttackData(currState).damage;
}

void PlayerAttackController::LoadAttackDuration()
{
    for (int i = 0; i < attackData.size(); i++)
    {
        SpriteSheet* sheet = playerController->GetSpriteSheetFromAnimator(static_cast<PlayerCombatState>(i));
        if (sheet != nullptr)
        {
            float duration = 0;
            for (int frame = 0; frame < sheet->frames.size() - 1; frame++)
            {
                duration += sheet->frames[frame].duration;
            }
            attackData[i].duration = duration;
            attackData[i].recovery = sheet->frames[sheet->frames.size() - 1].duration;
        }
    }
}

void PlayerAttackController::StartAttack(const PlayerCombatState combatState)
{
    if (combatState == combatStateMachine.GetCurrState())
    {
        return;
    }

    if (combatState == PlayerCombatState::DEFAULT)
    {
        return;
    }

    combatStateMachine.ChangeState(combatState);
    comboTimer = 0;
    comboInputBuffer = false;
    hasHit = false;

    const AttkData& data = GetAttackData(combatState);

    float hitboxOffsetX;
    float vfxOffsetX;

    if (ownerFacingRight)
    {
        hitboxOffsetX = data.hitboxOffsetX;
        vfxOffsetX = data.VFXOffsetX;
    }
    else
    {
        hitboxOffsetX = -data.hitboxOffsetX;
        vfxOffsetX = -data.VFXOffsetX;
    }

    if (GetCurrState() == PlayerCombatState::ATTK2)
    {
        AttackVFXManager::Instance().PlayAttackVFX(ownerTransform, Vector2(vfxOffsetX, data.VFXOffsetY),
                                               GetCurrState(), !ownerFacingRight, true);
    }
    else
    {
        AttackVFXManager::Instance().PlayAttackVFX(ownerTransform, Vector2(vfxOffsetX, data.VFXOffsetY),
                                                   GetCurrState(), !ownerFacingRight);
    }

    hitbox.Activate(ownerTransform->center.x, ownerTransform->center.y, hitboxOffsetX, data.hitboxOffsetY, data.width,
                    data.height, data.duration);
}

void PlayerAttackController::EndAttack()
{
    hitbox.isActive = false;
    comboInputBuffer = false;
    comboTimer = 0;
    hasHit = false;
    combatStateMachine.ChangeState(PlayerCombatState::DEFAULT);
}

void PlayerAttackController::AdvanceCombo(const PlayerCombatState currState, const AttkData& currData)
{
    if (currState == PlayerCombatState::ATTK0 && currData.recovery > 0)
    {
        StartAttack(PlayerCombatState::ATTK1);
        playerController->Dash(500, 0.04, false);
    }
    else if (currState == PlayerCombatState::ATTK1 && currData.recovery > 0)
    {
        StartAttack(PlayerCombatState::ATTK2);
        playerController->Dash(2000, 0.1, true);
    }
}

void PlayerAttackController::HandleAttackCollisions()
{
    if (!hitbox.isActive)
    {
        return;
    }

    for (const auto enemy : EnemyManager::Instance().GetActiveEnemies())
    {
        if (hitbox.CheckOverlap(enemy->GetPosition().x, enemy->GetPosition().y,
                                enemy->GetSize().x, enemy->GetSize().y))
        {
            enemy->TakeDamage(GetCurrentDamage());

        }
    }
}

const AttkData& PlayerAttackController::GetAttackData(const PlayerCombatState currState) const
{
    switch (currState)
    {
    case PlayerCombatState::ATTK0:
        return attackData[0];
    case PlayerCombatState::ATTK1:
        return attackData[1];
    case PlayerCombatState::ATTK2:
    default:
        return attackData[2];
    }
}
