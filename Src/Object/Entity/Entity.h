//
// Created by saber on 1/14/2026.
//

#ifndef HEW_ENTITY_H
#define HEW_ENTITY_H
#include "../Object.h"
#include "../../World/Camera.h"
#include "../../Config/Structs.h"
#include "../../World/Tilemap.h"

class Entity : public Object
{
public:
    Entity() = default;

    void Initialize(const EntityConfig& config);
    void Start() override;
    virtual void Update(float deltaTime, float trueDeltaTime, Tilemap& tileMap);
    void Draw(Camera& cam) override;

    virtual void TakeDamage(float inDamage);
    virtual void Die();

    virtual bool IsGrounded() { return isGrounded; }
    virtual bool IsFacingRight() { return isFacingRight; }
    [[nodiscard]] float GetCurrentHp() const { return currHp; }
    [[nodiscard]] float GetMaxHp() const { return maxHp; }

    /**
     *
     * @param knockBackForce
     * @param knockBackDirection true == right, false == left
     */
    virtual void TakeKnockback(float knockBackForce, bool knockBackDirection);

protected:
    virtual void HandleMovement(float deltaTime, Tilemap& tileMap);
    virtual void ApplyPhysics(float deltaTime);

    virtual bool CheckCollisionX(const Tilemap& tilemap, float newX);
    virtual bool CheckCollisionY(const Tilemap& tilemap, float newY);
    virtual bool CheckGrounded(const Tilemap& tilemap);

protected:
    float velX{};
    float velY{};

    float gravity{};
    float maxFallSpeed{};
    float checkGroundOffset{};

    float currHp{};
    float maxHp{};

    bool isGrounded = false;
    bool isFacingRight{};

    float currSpeedX;

    bool isKnockedBack = false;
    bool knockedBackDirection = false;
    float knockBackRecoveryDuration;
    float knockBackRecoveryTimer;

    bool isInvic;
    float invicCD;
    float invicTimer;
};


#endif //HEW_ENTITY_H
