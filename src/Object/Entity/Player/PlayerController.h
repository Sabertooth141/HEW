//
// Created by saber on 1/14/2026.
//

#ifndef HEW_PLAYERCONTROLLER_H
#define HEW_PLAYERCONTROLLER_H
#include "../Entity.h"
#include "../../../World/Camera.h"
#include "../../../World/Tilemap.h"


class Camera;

class PlayerController : public Entity
{
    // functions
public:
    PlayerController();

    void Start(float x, float y);
    void Update(float deltaTime, const Tilemap& tileMap) override;
    void Draw(const Camera& cam) override;
    void TakDamage() override;
    void Die() override;

private:
    void HandleMovement();

    // vars
private:
    float moveSpeed = 0;
};


#endif //HEW_PLAYERCONTROLLER_H
