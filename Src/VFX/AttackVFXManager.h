//
// Created by A.Aidus on 2/8/2026.
//

#ifndef HEW_ATTACKVFXMANAGER_H
#define HEW_ATTACKVFXMANAGER_H

#include "../Animation/Animator.h"
#include "../Config/Structs.h"

class Camera;
enum class EnemyState : unsigned char;
enum class PlayerCombatState : unsigned char;
class Animator;

enum class EnemyVFXType : unsigned char
{
    MINE,
};

class AttackVFXManager
{
public:
    static AttackVFXManager& Instance()
    {
        static AttackVFXManager instance;
        return instance;
    }

    AttackVFXManager() = default;

    AttackVFXManager& operator=(const AttackVFXManager&) = delete;

    void Update(float deltaTime);
    void Draw(const Camera& camera);

    void PlayAttackVFX(const Transform* playTransform, Vector2 offset, PlayerCombatState playerState,
                       bool flipHorizontal, bool attached = false);
    void PlayAttackVFX(const Transform* playTransform, Vector2 offset, EnemyVFXType enemyType, bool flipHorizontal,
                       bool attached = false);
    void InitAnimation(const PlayerCombatAnimPaths& path);
    void InitAnimation(const EnemyAnimPaths<EnemyVFXType>& path);

private:
    AttackVFXAnimators vfxAnimators{};

    std::vector<SplashTrailEffect> playingEffect{};
};


#endif //HEW_ATTACKVFXMANAGER_H
