//
// Created by saber on 2/10/2026.
//

#include "UGV.h"

void UGV::Update(const float deltaTime, Tilemap& tileMap)
{
    Enemy::Update(deltaTime, tileMap);
}

void UGV::HandleAttack(Entity* inTarget)
{
    Enemy::HandleAttack(inTarget);
}
