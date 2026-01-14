//
// Created by saber on 1/14/2026.
//

#ifndef HEW_PLAYERCONTROLLER_H
#define HEW_PLAYERCONTROLLER_H
#include "../Entity.h"


class PlayerController : public Entity
{
    // functions
public:
    void Start() override;
    void Update() override;
    void Draw() override;

    void TakDamage() override;
    void Die() override;

private:
    void HandleMovement();

    // vars
private:
    float _moveSpeed = 0;
};


#endif //HEW_PLAYERCONTROLLER_H
