//
// Created by saber on 1/28/2026.
//

#ifndef HEW_PLAYERATTACKCONTROLLER_H
#define HEW_PLAYERATTACKCONTROLLER_H
#include <vector>

#include "PlayerStates.h"
#include "../../../Config/Structs.h"
#include "../../../Util/Hitbox.h"

class PlayerController;

class PlayerAttackController
{
public:
    PlayerAttackController() = default;
    ~PlayerAttackController();

    void Initialize(const PlayerAttackConfig& config, PlayerController* controller);
    void Update(float deltaTime, Transform& playerTransform, bool isFacingRight);
    void LoadAttackDuration();
    void Shutdown();

    // returns true if can attack
    bool TryAttack();
    void CancelCombo();

    void StartRewindAttack(const Entity* targetEnemy);

    [[nodiscard]] Hitbox& GetHitBox() { return hitbox; }
    [[nodiscard]] const Hitbox& GetHitBox() const { return hitbox; }
    [[nodiscard]] PlayerCombatState GetCurrState() const { return combatStateMachine.GetCurrState(); }
    [[nodiscard]] bool IsAttacking() const;
	[[nodiscard]] bool IsInRecovery() const { return isInRecovery; }
    [[nodiscard]] bool CanMove() const;
    [[nodiscard]] float GetCurrentDamage() const;
    [[nodiscard]] bool IsRewindMoving() const { return rewindMoving; }
private:
    void StartAttack(PlayerCombatState combatState);
    void EndAttack();
    void AdvanceCombo(PlayerCombatState currState, const AttkData& currData);
    void HandleAttackCollisions();
    [[nodiscard]] const AttkData& GetAttackData(PlayerCombatState currState) const;

public:

private:
    PlayerStateMachine<PlayerCombatState> combatStateMachine{PlayerCombatState::DEFAULT};
    Hitbox hitbox{};

    std::vector<AttkData> attackData;
    PlayerController* playerController{};

    // combo tracking
    float comboTimer = 0.0f;
    bool comboInputBuffer = false;
    bool hasHit = false;
	bool isInRecovery = false;
    float rewindAttackDashStart = 0;
    float rewindAttackXOffset = 100;

    bool rewindAttackDashed = false;

    Vector2 rewindMoveTarget;
    bool rewindMoving = false;
    float rewindMoveSpeed = 1500;

    // owner position
    Transform* ownerTransform = nullptr;
    bool ownerFacingRight = true;

    // sfx
    std::vector<int*> attackSFX;
};

#endif //HEW_PLAYERATTACKCONTROLLER_H
