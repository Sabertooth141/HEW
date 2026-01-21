//
// Created by saber on 1/14/2026.
//

#ifndef HEW_PLAYERCONTROLLER_H
#define HEW_PLAYERCONTROLLER_H
#include <initializer_list>

#include "../Entity.h"
#include "../../../World/Camera.h"
#include "../../../World/Tilemap.h"

struct KeyBinding
{
    int primary = -1;
    int secondary = -1;

    [[nodiscard]] bool IsPressed() const
    {
        return ChkKeyPress(primary) || ChkKeyPress(secondary);
    }

    [[nodiscard]] bool IsEdge() const
    {
        return ChkKeyEdge(primary) || ChkKeyEdge(secondary);
    }
};

struct InputConfig
{
    KeyBinding moveLeft = {PK_LEFT, PK_A};
    KeyBinding moveRight = {PK_RIGHT, PK_D};
    KeyBinding jump = {PK_SP, PK_W};
    KeyBinding timeStop = {PK_J};
};

struct PlayerConfig : EntityConfig
{
    float walkSpeed;
    float sprintSpeed;
    float jumpForce;
    float timeStopDuration;
};

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

    // vars
private:
    float walkSpeed;
    float sprintSpeed;
    float jumpForce;
    float timeStopDuration;

    InputConfig input;
};


#endif //HEW_PLAYERCONTROLLER_H
