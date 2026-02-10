//
// Created by saber on 1/14/2026.
//

#ifndef HEW_PLAYERCONTROLLER_H
#define HEW_PLAYERCONTROLLER_H

#include <string>

#include "PlayerAttackController.h"
#include "../../../Animation/Animator.h"
#include "PlayerStates.h"
#include "../Entity.h"

#define SPEED_WHEN_ATTK 10
#define GRAV_WHEN_ATTK 20

class Camera;

struct PlayerNormalAnimPaths
{
    PlayerNormalState animationState;
    std::string jsonPath;
    std::string bmpPath;
    int startFrame = 0;
};

class PlayerController final : public Entity
{
    // functions
public:
    PlayerController();

    void Initialize(const PlayerConfig& config, const PlayerAttackConfig& attackConfig);
    void Start() override;
    void Update(float deltaTime, Tilemap& tileMap) override;
    void Draw(const Camera& cam) override;
    void Die() override;
    void InitAnimation(const PlayerNormalAnimPaths& path);
    void InitAttackAnimation(const PlayerCombatAnimPaths& path);
    void Dash(float dashVel, float inDashDuration, bool isInvic);
    void TakeDamage(float inDamage) override;

    SpriteSheet* GetSpriteSheetFromAnimator(PlayerCombatState attkState);
    [[nodiscard]] PlayerStateMachine<PlayerNormalState> GetMoveStateMachine() const { return normalStateMachine; }
    [[nodiscard]] const PlayerAttackController& GetAttackController() const { return attackController; }

protected:
    void HandleMovement(float deltaTime, Tilemap& tileMap) override;
    void ApplyPhysics(float deltaTime) override;

private:
    void HandleInput(float deltaTime);
    void HandleTimeRewind();
    void HandleAnimationUpdate(float deltaTime);

    // vars
private:
    float walkSpeed;
    float normalDashSpeed;
    float currDashSpeed;
    float jumpForce;
    float timeStopDuration;
    float airResistance;
    float normalGravity;
    bool isDamageable;
    int8_t isFacingRightBuffer = -1; // 1 RIGHT 0 LEFT -1 UNDEFINED

    // dash
    float dashTimer;
    float normalDashDuration;
    float currDashDuration;
    bool isDashing;

    InputConfig input;
    PlayerSnapshot snapshot;

    PlayerStateMachine<PlayerNormalState> normalStateMachine;
    PlayerAttackController attackController;

    // animations
    PlayerAnimators playerAnimators;
    Animator* animatorPlaying;
};


#endif //HEW_PLAYERCONTROLLER_H
