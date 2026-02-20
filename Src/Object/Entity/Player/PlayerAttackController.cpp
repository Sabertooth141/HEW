//
// Created by saber on 1/28/2026.
//

#include "PlayerAttackController.h"

#include "../../../Systems/EnemyManager.h"
#include "PlayerController.h"
#include "../../../Lib/conioex_custom.h"
#include "../../../VFX/AttackVFXManager.h"

PlayerAttackController::~PlayerAttackController()
{
    Shutdown();
}

void PlayerAttackController::Initialize(const PlayerAttackConfig& config, PlayerController* controller)
{
    attackSFX.assign(4, nullptr);

    attackSFX[0] = MciOpenSound("../Assets/Audio/Attk0.mp3");
    attackSFX[1] = MciOpenSound("../Assets/Audio/Attk1.mp3");
    attackSFX[2] = MciOpenSound("../Assets/Audio/Attk2.mp3");
    attackSFX[3] = MciOpenSound("../Assets/Audio/Attk3.mp3");

    attackData = config.data;

    playerController = controller;
}

void PlayerAttackController::Update(const float deltaTime, Transform& playerTransform,
                                    const bool isFacingRight)
{
    HandleAttackCollisions();

    ownerTransform = &playerTransform;
    if (combatStateMachine.GetCurrState() != PlayerCombatState::REWIND_ATTK)
    {
        ownerFacingRight = isFacingRight;
    }

    combatStateMachine.Update(deltaTime);
    comboTimer += deltaTime;

    const PlayerCombatState currentState = combatStateMachine.GetCurrState();

    if (currentState == PlayerCombatState::DEFAULT)
    {
        return;
    }

    if (currentState == PlayerCombatState::REWIND_ATTK)
    {
        if (rewindMoving)
        {
            float dx = rewindMoveTarget.x - ownerTransform->center.x;

            float frameMove = rewindMoveSpeed * deltaTime;

            if (std::abs(dx) <= frameMove || comboTimer >= rewindAttackDashStart)
            {
                rewindMoving = false;
                playerController->SetVelocityX(0);
            }
            else
            {
                float direction;
                if (dx > 0)
                {
                    direction = 1;
                }
                else
                {
                    direction = -1;
                }

                playerController->SetVelocityX(direction * rewindMoveSpeed);
            }
        }
        if (comboTimer >= rewindAttackDashStart && !rewindAttackDashed)
        {
            rewindAttackDashed = true;
            const AttkData& data = GetAttackData(PlayerCombatState::REWIND_ATTK);

            float hitboxOffsetX = ownerFacingRight ? data.hitboxOffsetX : -data.hitboxOffsetX;
            float vfxOffsetX = ownerFacingRight ? data.VFXOffsetX : -data.VFXOffsetX;

            AttackVFXManager::Instance().PlayAttackVFX(
                ownerTransform, Vector2(vfxOffsetX, data.VFXOffsetY),
                PlayerCombatState::ATTK2, !ownerFacingRight, true);
            hitbox.Activate(ownerTransform->center.x, ownerTransform->center.y,
                            hitboxOffsetX, data.hitboxOffsetY,
                            data.width, data.height, data.duration - data.recovery);

            playerController->Dash(2000, 0.1, true);
            MciPlaySound(attackSFX[3], 0);
        }
    }

    const AttkData& data = GetAttackData(currentState);
    hitbox.Update(deltaTime, ownerTransform->center.x, ownerTransform->center.y);

    // attack
    if (comboTimer <= data.duration)
    {
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

    if (currState == PlayerCombatState::REWIND_ATTK)
    {
        return false;
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

void PlayerAttackController::StartRewindAttack(const Entity* targetEnemy)
{
    if (targetEnemy == nullptr)
    {
        return;
    }

    float enemyCenterX = targetEnemy->transform.center.x;
    float playerCenterX = ownerTransform->center.x;

    float leftPos = enemyCenterX - rewindAttackXOffset;
    float rightPos = enemyCenterX + rewindAttackXOffset;

    float distToLeft = std::abs(playerCenterX - leftPos);
    float distToRight = std::abs(playerCenterX - rightPos);

    if (distToLeft < distToRight)
    {
        rewindMoveTarget.x = leftPos;
        ownerFacingRight = true;
    }
    else
    {
        rewindMoveTarget.x = rightPos;
        ownerFacingRight = false;
    }

    playerController->SetIsFacingRight(ownerFacingRight);
    rewindMoveTarget.y = ownerTransform->center.y;
    rewindMoving = true;

    combatStateMachine.ChangeState(PlayerCombatState::REWIND_ATTK);
    comboTimer = 0;
    comboTimer = 0;
    comboInputBuffer = false;
    hasHit = false;

    Camera::Instance().SetLetterbox(0.3);
    playerController->SetDamageable(false);
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

                if (i == 3 && frame < 4)
                {
                    rewindAttackDashStart += sheet->frames[frame].duration;
                }
            }
            attackData[i].duration = duration;
            attackData[i].recovery = sheet->frames[sheet->frames.size() - 1].duration;
        }
    }

    DebugPrintf("%f", rewindAttackDashStart);
}

void PlayerAttackController::Shutdown()
{
    for (const auto sfx : attackSFX)
    {
        MciStopSound(sfx);
        MciCloseSound(sfx);
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

    MciPlaySound(attackSFX[static_cast<int>(combatStateMachine.GetCurrState())], 0);
    hitbox.Activate(ownerTransform->center.x, ownerTransform->center.y, hitboxOffsetX, data.hitboxOffsetY, data.width,
                    data.height, data.duration - data.recovery);
}

void PlayerAttackController::EndAttack()
{
    hitbox.isActive = false;
    comboInputBuffer = false;
    comboTimer = 0;
    hasHit = false;
    isInRecovery = false;
    playerController->SetDamageable(true);
    if (combatStateMachine.GetCurrState() == PlayerCombatState::REWIND_ATTK)
    {
        rewindMoving = false;
        TimeManager::Instance().DeactivateTimeStop();
        Camera::Instance().SetLetterbox(0);
    }
    rewindAttackDashed = false;
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
        return attackData[2];
    case PlayerCombatState::REWIND_ATTK:
        return attackData[3];
    default:
        return attackData[2];
    }
}
