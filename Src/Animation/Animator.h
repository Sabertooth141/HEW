//
// Created by saber on 1/28/2026.
//

#ifndef HEW_ANIMATOR_H
#define HEW_ANIMATOR_H

#include <memory>
#include <unordered_map>

#include "SpriteFrame.h"
#include "../Object/Entity/Enemy/EnemyStates.h"
#include "../Object/Entity/Player/PlayerStates.h"

enum class EnemyVFXType : unsigned char;
enum class PlayerCombatState : uint8_t;
enum class PlayerNormalState : uint8_t;
class Camera;

class Animator
{
public:
    Animator();
    ~Animator();

    bool LoadSpriteSheet(const char* jsonFile, const char* bmpFile);

    // Animation
    void Play(bool loop = true);
    void PlayFromFrame(int inStartFrame, int inEndFrame, bool loop = true);
    void Stop();
    void Pause();
    void Resume();

    void Update(float deltaTime);
    void Draw(const Camera& cam, float worldX, float worldY, bool flipHorizontal = false) const;
    void SetLoopStartFrame(int frameNo);

    void SetAnimStartFrame(int frameNo);
    void SetAnimEndFrame(int frameNo);

    [[nodiscard]] SpriteSheet* GetSpriteSheet() const { return spriteSheet; }

    [[nodiscard]] bool IsPlaying() const { return isPlaying; }
    [[nodiscard]] int GetCurrentFrameIndex() const { return currFrame; }
    [[nodiscard]] SpriteFrame* GetCurrentFrame() const;

    [[nodiscard]] int GetFrameWidth() const;
    [[nodiscard]] int GetFrameHeight() const;

private:
    void AdvanceFrame();

private:
    SpriteSheet* spriteSheet;

    int currFrame;
    int startFrame;
    int loopStartFrame;
    int endFrame;

    float frameTimer;
    bool isPlaying;
    bool isPaused;
    bool isLoop;

    bool isDirectionForward;
};

struct PlayerAnimators
{
    std::unordered_map<PlayerNormalState, std::unique_ptr<Animator>> moveAnimators;
    std::unordered_map<PlayerCombatState, std::unique_ptr<Animator>> combatAnimators;

    bool AddAnimator(const PlayerNormalState animationName, std::unique_ptr<Animator> animator)
    {
        if (animator == nullptr)
        {
            return false;
        }

        if (moveAnimators.contains(animationName))
        {
            return false;
        }

        moveAnimators[animationName] = std::move(animator);

        return true;
    }

    bool AddAnimator(const PlayerCombatState animationName, std::unique_ptr<Animator> animator)
    {
        if (animator == nullptr)
        {
            return false;
        }

        if (combatAnimators.contains(animationName))
        {
            return false;
        }

        combatAnimators[animationName] = std::move(animator);

        return true;
    }

    Animator* GetAnimator(const PlayerNormalState animationName)
    {
        auto it = moveAnimators.find(animationName);
        if (it != moveAnimators.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    Animator* GetAnimator(const PlayerCombatState animationName)
    {
        auto it = combatAnimators.find(animationName);
        if (it != combatAnimators.end())
        {
            return it->second.get();
        }
        return nullptr;
    }
};

struct EnemyAnimators
{
    std::unordered_map<EnemyState, std::unique_ptr<Animator>> animators;

    bool AddAnimator(const EnemyState animationName, std::unique_ptr<Animator> animator)
    {
        if (animator == nullptr)
        {
            return false;
        }

        if (animators.contains(animationName))
        {
            return false;
        }

        animators[animationName] = std::move(animator);

        return true;
    }

    Animator* GetAnimator(const EnemyState animationName)
    {
        auto it = animators.find(animationName);
        if (it != animators.end())
        {
            return it->second.get();
        }
        return nullptr;
    }
};

struct AttackVFXAnimators
{
    std::unordered_map<PlayerCombatState, std::unique_ptr<Animator>> playerAnimators;
    std::unordered_map<EnemyVFXType, std::unique_ptr<Animator>> enemyAnimators;

    bool AddAnimator(const PlayerCombatState animationName, std::unique_ptr<Animator> animator)
    {
        if (animator == nullptr)
        {
            return false;
        }

        if (playerAnimators.contains(animationName))
        {
            return false;
        }

        playerAnimators[animationName] = std::move(animator);

        return true;
    }

    bool AddAnimator(const EnemyVFXType animationName, std::unique_ptr<Animator> animator)
    {
        if (animator == nullptr)
        {
            return false;
        }

        if (enemyAnimators.contains(animationName))
        {
            return false;
        }

        enemyAnimators[animationName] = std::move(animator);

        return true;
    }

    Animator* GetAnimator(const PlayerCombatState animationName)
    {
        auto it = playerAnimators.find(animationName);
        if (it != playerAnimators.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    Animator* GetAnimator(const EnemyVFXType animationName)
    {
        auto it = enemyAnimators.find(animationName);
        if (it != enemyAnimators.end())
        {
            return it->second.get();
        }
        return nullptr;
    }
};

#endif //HEW_ANIMATOR_H