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
#include "../../../Systems/TimeManager.h"
#include "../../../Config/Input.h"

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
    void Update(float deltaTime, float trueDeltaTime, Tilemap& tileMap) override;
    void Draw(Camera& cam) override;
    void Die() override;
    void InitAnimation(const PlayerNormalAnimPaths& path);
    void InitAttackAnimation(const PlayerCombatAnimPaths& path);
    void Dash(float dashVel, float inDashDuration, bool isInvic);
    void TakeDamage(float inDamage) override;
    void SetDamageable(bool inDamageable);

    void SetIsFacingRight(const bool inFacingRight) { isFacingRight = inFacingRight; }
    void SetVelocityX(const float vx) { velX = vx; }
    SpriteSheet* GetSpriteSheetFromAnimator(PlayerCombatState attkState);
    [[nodiscard]] PlayerStateMachine<PlayerNormalState> GetMoveStateMachine() const { return normalStateMachine; }
    [[nodiscard]] const PlayerAttackController& GetAttackController() const { return attackController; }

protected:
    void HandleMovement(float deltaTime, Tilemap& tileMap) override;
    void ApplyPhysics(float deltaTime) override;
    void TakeKnockback(float knockBackForce, bool knockBackDirection) override;

private:
    void HandleInput(float deltaTime);
    void HandleAnimationUpdate(float deltaTime);
    void DrawTrail(const Camera& cam) const;
    void DrawRewind(const Camera& cam) const;
    void TriggerRewindAttack(TimeManager::RewindAttackFrame target);

    void DrawTrailFromSnapshot(const Camera& cam, const PlayerSnapshot& snapshotToDraw, const RGBQUAD& trailColor,
                               float fadeMult) const;

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

    float trailFadeTimer = 0;
    float trailFadeDuration = 0;

    float rewindAttackDamage = 60;

    bool isMovingInput = false;
    bool isDrawTrail = false;

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
    PlayerSnapshot rewindSnapshot;

    // flicker when taking damage
    int flickerCounter = 0;
    int flickerInterval = 4;
    bool flickerVisible = true;

};


#endif //HEW_PLAYERCONTROLLER_H
