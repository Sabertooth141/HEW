//
// Created by saber on 1/28/2026.
//

#include "PlayerAttackController.h"

#include "../../../Systems/EnemyManager.h"
#include "PlayerController.h"

void PlayerAttackController::Initialize(const PlayerAttackConfig& config)
{
    attackData = config.data;

    attackData.back().comboWindow = 0;
}

void PlayerAttackController::Update(const float deltaTime, const Transform& playerTransform,
                                    const bool isFacingRight)
{
    HandleAttackCollisions();

    ownerCenterX = playerTransform.center.x;
    ownerCenterY = playerTransform.center.y;
    ownerFacingRight = isFacingRight;

    combatStateMachine.Update(deltaTime);
    comboTimer += deltaTime;

    const PlayerCombatState currentState = combatStateMachine.GetCurrState();

    if (currentState == PlayerCombatState::DEFAULT)
    {
        return;
    }

    const AttkData& data = GetAttackData(currentState);
    hitbox.Update(deltaTime, ownerCenterX, ownerCenterY);

    HandleAnimationUpdate(deltaTime);

    // attack
    if (comboTimer <= data.duration)
    {
        char debug[64];
        sprintf_s(debug, "CurrentAttk: %d\n", currentState);
        OutputDebugStringA(debug);
        // hitbox.Activate(ownerX, ownerY, data.offsetX, data.offsetY,
        //                 data.width, data.height, data.duration);
    }
    // if combo buffered
    else if (comboTimer <= data.duration + data.recovery)
    {
        if (hitbox.isActive)
        {
            hitbox.isActive = false;
        }

        if (comboInputBuffer && comboTimer >= data.duration + data.recovery * 0.3f)
        {
            comboInputBuffer = false;

            AdvanceCombo(currentState, data);
        }
    }
    // if combo pressed within window
    else if (comboTimer <= data.duration + data.recovery + data.comboWindow)
    {
        if (comboInputBuffer)
        {
            comboInputBuffer = false;

            AdvanceCombo(currentState, data);
        }
    }
    // complete attack
    else
    {
        EndAttack();
    }
}

void PlayerAttackController::InitAnimation(const PlayerCombatAnimPaths& path)
{
    std::unique_ptr<Animator> animator = std::make_unique<Animator>();
    animator->LoadSpriteSheet(path.jsonPath.c_str(), path.bmpPath.c_str());
    animator->SetLoopStartFrame(path.startFrame);

    combatAnimators.AddAnimator(path.animationState, std::move(animator));
}

void PlayerAttackController::Draw(Camera& cam)
{
    if (animatorPlaying != nullptr)
    {
        // animatorPlaying->Draw(cam, transform.topLeft.x, transform.topLeft.y, !isFacingRight);
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
    hitbox.isActive = false;
    comboInputBuffer = false;
    comboTimer = 0;
    hasHit = false;
    combatStateMachine.ChangeState(PlayerCombatState::DEFAULT);
}

bool PlayerAttackController::IsAttacking() const
{
    return combatStateMachine.GetCurrState() != PlayerCombatState::DEFAULT;
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


    if (ownerFacingRight)
    {
        hitboxOffsetX = data.offsetX;
    }
    else
    {
        hitboxOffsetX = -data.offsetX;
    }

    hitbox.Activate(ownerCenterX, ownerCenterY, hitboxOffsetX, data.offsetY, data.width, data.height, data.duration);
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
    if (currState == PlayerCombatState::ATTK0 && currData.comboWindow > 0)
    {
        StartAttack(PlayerCombatState::ATTK1);
    }
    else if (currState == PlayerCombatState::ATTK1 && currData.comboWindow > 0)
    {
        StartAttack(PlayerCombatState::ATTK2);
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
            hitbox.isActive = false;
            break;
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

void PlayerAttackController::HandleAnimationUpdate(const float deltaTime)
{
    if (IsAttacking())
    {
        if (animatorPlaying != nullptr && animatorPlaying == combatAnimators.GetAnimator(combatStateMachine.GetCurrState()))
        {
            animatorPlaying->Update(deltaTime);
            return;
        }

        if (animatorPlaying != nullptr)
        {
            animatorPlaying->Stop();
        }

        animatorPlaying = combatAnimators.GetAnimator(combatStateMachine.GetCurrState());
        if (animatorPlaying == nullptr)
        {
            animatorPlaying = combatAnimators.GetAnimator(PlayerCombatState::ATTK1);
        }
        animatorPlaying->Play();
        animatorPlaying->Update(deltaTime);
    }
}
