//
// Created by saber on 2/10/2026.
//

#ifndef HEW_UGV_H
#define HEW_UGV_H
#include "../Enemy.h"

class UGV final : public Enemy
{
public:
    UGV() = default;

    void Initialize(const UGVConfig& config);
    void Update(float deltaTime, Tilemap& tileMap) override;

protected:
    void HandleAttack(Entity* inTarget) override;
    void HandleAnimationUpdate(float deltaTime) override;

private:
    void HandleAttackWindUp();
    void HandleAttackKnockBack();

private:
    float knockBackForce;

    float attackWindUpTimer;
    float attackWindUpDuration;
};


#endif //HEW_UGV_H