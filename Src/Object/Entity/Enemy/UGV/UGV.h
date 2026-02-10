//
// Created by saber on 2/10/2026.
//

#ifndef HEW_UGV_H
#define HEW_UGV_H
#include "../Enemy.h"

class UGV final : public Enemy
{
protected:
    void Update(float deltaTime, Tilemap& tileMap) override;
    void HandleAttack(Entity* inTarget) override;

private:
    void HandleAttackWindUp();
    void HandleAttackKnockBack();

private:

};


#endif //HEW_UGV_H