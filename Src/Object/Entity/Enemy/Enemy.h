//
// Created by saber on 1/22/2026.
//

#ifndef HEW_ENEMY_H
#define HEW_ENEMY_H
#include "EnemyStates.h"
#include "../Entity.h"

class Enemy : public Entity
{
public:
    Enemy();

    void Initialize(const EnemyConfig& config);
    void Start() override;
    void Update(float deltaTime, const Tilemap& tileMap) override;
    void Draw(const Camera& cam) override;
protected:
    void HandleMovement(float deltaTime, const Tilemap& tilemap) override;
    virtual void HandleAttack(Vector2 targetPos);

    void Die() override;

private:
    void TargetDetection();

protected:
    Transform* targetTransform;
    float attackCooldown;
    float moveSpeed;
    float damage;

    EnemyStateMachine stateMachine;
};


#endif //HEW_ENEMY_H