//
// Created by A.Aidus on 2/8/2026.
//

#ifndef HEW_ATTACKVFXMANAGER_H
#define HEW_ATTACKVFXMANAGER_H

enum class EnemyState : unsigned char;
enum class PlayerCombatState : unsigned char;
class Animator;

class AttackVFXManager
{
public:
	AttackVFXManager() = default;

	PlayAttackVFX(float inX, float inY, PlayerCombatState playerState);
	PlayAttackVFX(float inX, float inY, EnemyState enemyState);
private:
	Animator* vfxAnimator;

};


#endif //HEW_ATTACKVFXMANAGER_H