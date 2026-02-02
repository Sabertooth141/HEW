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
    void Update(float deltaTime, const Tilemap& tileMap) override;
    void Draw(const Camera& cam) override;
    void Die() override;
    void InitAnimation(const PlayerNormalAnimPaths& path);

    [[nodiscard]] PlayerStateMachine<PlayerNormalState> GetMoveStateMachine() const { return normalStateMachine; }
    [[nodiscard]] PlayerAttackController GetAttackController() const { return attackController; }

protected:
    void HandleMovement(float deltaTime, const Tilemap& tileMap) override;
    void ApplyPhysics(float deltaTime) override;

private:
    void HandleInput(float deltaTime);
    void HandleTimeRewind();
    void HandleAnimationUpdate(float deltaTime);

    // vars
private:
    float walkSpeed;
    float sprintSpeed;
    float jumpForce;
    float timeStopDuration;
    float airResistance;
    float normalGravity;

    InputConfig input;
    PlayerSnapshot snapshot;

    PlayerStateMachine<PlayerNormalState> normalStateMachine;
    PlayerAttackController attackController;

    // animations
    PlayerAnimators playerAnimators;
    Animator* animatorPlaying;
};


#endif //HEW_PLAYERCONTROLLER_H
