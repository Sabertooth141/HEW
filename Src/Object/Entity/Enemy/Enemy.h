//
// Created by saber on 1/22/2026.
//

#ifndef HEW_ENEMY_H
#define HEW_ENEMY_H
#include <string>

#include "EnemyStates.h"
#include "../Entity.h"

struct EnemyAnimPaths
{
    EnemyState animationState;
    std::string jsonPath;
    std::string bmpPath;
    int startFrame = 0;
};

class Enemy : public Entity
{
public:
    Enemy();

    void Initialize(const EnemyConfig& config);
    void Start() override;
    void Update(float deltaTime, const Tilemap& tileMap) override;
    void Draw(const Camera& cam) override;
    void TakeDamage(float inDamage) override;
    void InitAnimation(const EnemyAnimPaths& path);
protected:
    void HandleMovement(float deltaTime, const Tilemap& tilemap) override;
    virtual void HandleAttack(Entity* inTarget);
    virtual void HandleAnimationUpdate(float deltaTime);

    void Die() override;
    // to be overridden by children
    virtual bool DetectTarget(float deltaTime);

protected:
    Entity* target;
    float attackCooldown;
    float moveSpeed;
    float damage;

    float invicCD;
    float invicTimer;

    Animator* animatorPlaying = nullptr;
    EnemyAnimators animators;

    EnemyStateMachine stateMachine;
};


#endif //HEW_ENEMY_H