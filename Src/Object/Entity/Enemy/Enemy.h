//
// Created by saber on 1/22/2026.
//

#ifndef HEW_ENEMY_H
#define HEW_ENEMY_H
#include "../Entity.h"
#include "../../../Config/Structs.h"

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

    void Die() override;

private:
    void TargetDetection();

private:
    TargetPosition target;

    float attackCooldown;
    float moveSpeed;
};


#endif //HEW_ENEMY_H