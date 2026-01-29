//
// Created by saber on 1/14/2026.
//

#ifndef HEW_PLAYERCONTROLLER_H
#define HEW_PLAYERCONTROLLER_H

#include "PlayerStates.h"
#include "PlayerAttackController.h"
#include "../Entity.h"
#include "../../../World/Camera.h"
#include "../../../Config/Structs.h"

class Camera;

class PlayerController final : public Entity
{
    // functions
public:
    PlayerController();

    void Initialize(const PlayerConfig& config, const PlayerAttackConfig& attackConfig) ;
    void Start() override;
    void Update(float deltaTime, const Tilemap& tileMap) override;
    void Draw(const Camera& cam) override;
    void Die() override;

    [[nodiscard]] PlayerStateMachine<PlayerMoveState> GetMoveStateMachine() const { return moveStateMachine; }

protected:
    void HandleMovement(float deltaTime, const Tilemap& tileMap) override;
    void ApplyPhysics(float deltaTime) override;

private:
    void HandleInput(float deltaTime);
    void HandleTimeRewind();
    void InitAnimations();

    // vars
private:
    float walkSpeed;
    float sprintSpeed;
    float jumpForce;
    float timeStopDuration;
    float airResistance;


    InputConfig input;
    PlayerSnapshot snapshot;

    PlayerStateMachine<PlayerMoveState> moveStateMachine;
    PlayerAttackController attackController;

    PlayerAnimation animations;
};


#endif //HEW_PLAYERCONTROLLER_H
