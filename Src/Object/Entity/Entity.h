//
// Created by saber on 1/14/2026.
//

#ifndef HEW_ENTITY_H
#define HEW_ENTITY_H
#include "../Object.h"
#include "../../World/Camera.h"
#include "../../World/Tilemap.h"

struct EntityConfig : ObjectConfig
{
    float velX;
    float velY;
    float gravity;
    float maxFallSpeed;
    float currHp;
    float maxHp;
    bool isFacingRight;
};

class Camera;
class Tilemap;

class Entity : public Object
{
public:
    Entity();

    void Initialize(const EntityConfig& config);
    void Start() override;
    void Update(float deltaTime, const Tilemap& tileMap) override;
    void Draw(const Camera& cam) override;

    virtual void HandleMovement(float deltaTime, const Tilemap& tileMap);

    virtual void TakDamage(float inDamage);
    virtual void Die();

    virtual bool IsGrounded() { return isGrounded; }
    virtual bool IsFacingRight() { return isFacingRight; }

protected:
    virtual void ApplyPhysics(float deltaTime);

    virtual bool CheckCollisionX(const Tilemap& tilemap, float newX);
    virtual bool CheckCollisionY(const Tilemap& tilemap, float newY);
    virtual bool CheckGrounded(const Tilemap& tilemap);

protected:
    float velX{};
    float velY{};

    float gravity{};
    float maxFallSpeed{};

    float currHp{};
    float maxHp{};

    bool isGrounded = false;
    bool isFacingRight{};

    float currSpeed{};
};


#endif //HEW_ENTITY_H
