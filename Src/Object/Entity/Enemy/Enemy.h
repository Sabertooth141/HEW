//
// Created by saber on 1/22/2026.
//

#ifndef HEW_ENEMY_H
#define HEW_ENEMY_H
#include "../Entity.h"


class Enemy : public Entity
{
public:
    Enemy();

    void Initialize(const ObjectConfig& config) override;
    void Start() override;
    void Update(float deltaTime, const Tilemap& tileMap) override;
    void Draw(const Camera& cam) override;
protected:
    void HandleMovement(float deltaTime, const Tilemap& tilemap) override;

private:
    void TargetDetection();

private:
    Entity target;
};


#endif //HEW_ENEMY_H