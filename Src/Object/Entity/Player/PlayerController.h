//
// Created by saber on 1/14/2026.
//

#ifndef HEW_PLAYERCONTROLLER_H
#define HEW_PLAYERCONTROLLER_H
#include <initializer_list>

#include "../Entity.h"
#include "../../../World/Camera.h"
#include "../../../Config/Structs.h"

class Camera;

class PlayerController final : public Entity
{
    // functions
public:
    PlayerController();

    void Initialize(const PlayerConfig& config);
    void Start() override;
    void Update(float deltaTime, const Tilemap& tileMap) override;
    void Draw(const Camera& cam) override;
    void Die() override;
private:
    void HandleInput();
    void HandleTimeRewind();

    // vars
private:
    float walkSpeed;
    float sprintSpeed;
    float jumpForce;
    float timeStopDuration;

    InputConfig input;
    PlayerSnapshot snapshot;
};


#endif //HEW_PLAYERCONTROLLER_H
