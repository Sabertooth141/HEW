//
// Created by saber on 1/22/2026.
//

#ifndef HEW_ENEMY_H
#define HEW_ENEMY_H

#include "EnemyStates.h"
#include "../Entity.h"
#include "../../../Config/Structs.h"

class Enemy : public Entity
{
public:
    Enemy();

    void Initialize(const EnemyConfig& config);
    void Start() override;
    void Update(float deltaTime, Tilemap& tileMap) override;
    void Draw(Camera& cam) override;
    void TakeDamage(float inDamage) override;
    void InitAnimation(const EnemyAnimPaths<EnemyState>& path);
    Entity* GetTarget() const { return target; }

protected:
    void HandleMovement(float deltaTime, Tilemap& tilemap) override;
    virtual void HandleAttack(Entity* inTarget);
    virtual void HandleAnimationUpdate(float deltaTime);

    void Die() override;
    // to be overridden by children
    virtual bool CanStartPathfinding(float deltaTime);
    virtual bool CanStartAttack(float deltaTime);

    virtual void HandlePatrol (Tilemap& tilemap, float deltaTime);
    virtual void PathfindToTarget(float deltaTime);

protected:
    Entity* target;

    float moveSpeed;
    float damage;

    bool canAttack;
    float attackCooldown;
    float attackCooldownTimer;
    bool isAttacking;
    float attackDistance;
    float detectionDistance;

    bool isInvic;
    float invicCD;
    float invicTimer;

    Animator* animatorPlaying = nullptr;
    EnemyAnimators animators;

    EnemyStateMachine stateMachine;
};


#endif //HEW_ENEMY_H