//
// Created by saber on 2/3/2026.
//

#ifndef HEW_MINE_H
#define HEW_MINE_H
#include "../Enemy.h"


class Mine final : public Enemy
{
public:
    Mine() = default;

    void Start() override;
    void Initialize(const MineConfig& config);
    void Update(float deltaTime, Tilemap& tileMap) override;
    void Draw(const Camera& cam) override;
    void HandleMovement(float deltaTime, Tilemap& tileMap) override;
    void HandleAttack(Entity* inTarget) override;

protected:
    bool CanStartAttack(float deltaTime) override;
private:
    void HandleExplosion(float deltaTime);

    void SetTimeToExplode();

private:
    float elapsedTime = 0.0f;
    float explodeTimer;
    float timeToExplode;

    float currIndicatorRadius;
    float explosionRadius;

    bool isExploding;
    bool canExplode;
};


#endif //HEW_MINE_H