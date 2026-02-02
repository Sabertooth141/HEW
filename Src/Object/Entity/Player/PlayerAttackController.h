//
// Created by saber on 1/28/2026.
//

#ifndef HEW_PLAYERATTACKCONTROLLER_H
#define HEW_PLAYERATTACKCONTROLLER_H
#include <string>
#include <vector>

#include "PlayerStates.h"
#include "../../../Config/Structs.h"
#include "../../../Util/Hitbox.h"

class PlayerController;

struct PlayerCombatAnimPaths
{
    PlayerCombatState animationState;
    std::string jsonPath;
    std::string bmpPath;
    int startFrame = 0;
};

class PlayerAttackController
{
public:
    PlayerAttackController() = default;

    void Initialize(const PlayerAttackConfig& config);
    void Update(float deltaTime, const Transform& playerTransform, bool isFacingRight);
    void InitAnimation(const PlayerCombatAnimPaths& path);
    void Draw(Camera& camera);

    // returns true if can attack
    bool TryAttack();
    void CancelCombo();

    [[nodiscard]] Hitbox& GetHitBox() { return hitbox; }
    [[nodiscard]] const Hitbox& GetHitBox() const { return hitbox; }
    [[nodiscard]] PlayerCombatState GetCurrState() const { return combatStateMachine.GetCurrState(); }
    [[nodiscard]] bool IsAttacking() const;
    [[nodiscard]] bool CanMove() const;
    [[nodiscard]] float GetCurrentDamage() const;
private:
    void LoadAttackDuration();
    void StartAttack(PlayerCombatState combatState);
    void EndAttack();
    void AdvanceCombo(PlayerCombatState currState, const AttkData& currData);
    void HandleAttackCollisions();
    [[nodiscard]] const AttkData& GetAttackData(PlayerCombatState currState) const;

private:
    PlayerStateMachine<PlayerCombatState> combatStateMachine{PlayerCombatState::DEFAULT};
    void HandleAnimationUpdate(float deltaTime);
    Hitbox hitbox{};

    PlayerController* playerController{};

    std::vector<AttkData> attackData;

    PlayerAnimators combatAnimators;
    Animator* animatorPlaying;

    // combo tracking
    float comboTimer = 0.0f;
    bool comboInputBuffer = false;
    bool hasHit = false;

    // owner position
    float ownerCenterX = 0;
    float ownerCenterY = 0;
    bool ownerFacingRight = true;
};


#endif //HEW_PLAYERATTACKCONTROLLER_H
