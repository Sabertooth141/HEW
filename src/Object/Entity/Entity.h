//
// Created by saber on 1/14/2026.
//

#ifndef HEW_ENTITY_H
#define HEW_ENTITY_H
#include "../Object.h"
#include "../../World/Camera.h"
#include "../../World/Tilemap.h"


class Camera;
class Tilemap;

class Entity : public Object
{
public:
    Entity();

    virtual void Start(float x, float y);

    virtual void Update(float deltaTime, const Tilemap& tileMap);

    virtual void Draw(const Camera& cam);

    virtual void TakDamage();
    virtual void Die();

    virtual bool IsGrounded() { return isGrounded; }
    virtual bool IsFacingRight() { return isFacingRight; }

protected:
    virtual void ApplyPhysics(float deltaTime);

    virtual bool CheckCollisionX(const Tilemap& tilemap, float newX);
    virtual bool CheckCollisionY(const Tilemap& tilemap, float newY);
    virtual bool CheckGrounded(const Tilemap& tilemap);

protected:
    float currHp;
    float maxHp;

    bool isGrounded;
    bool isFacingRight;
};


#endif //HEW_ENTITY_H