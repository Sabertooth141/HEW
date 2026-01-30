//
// Created by saber on 1/28/2026.
//

#ifndef HEW_ANIMATOR_H
#define HEW_ANIMATOR_H

#include <memory>
#include <unordered_map>

#include "SpriteFrame.h"
#include "../Object/Entity/Player/PlayerStates.h"

enum class PlayerCombatState;
enum class PlayerMoveState;
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
    std::unordered_map<PlayerMoveState, std::unique_ptr<Animator>> moveAnimators;
    std::unordered_map<PlayerCombatState, std::unique_ptr<Animator>> combatAnimators;

    bool AddAnimator(const PlayerMoveState animationName, std::unique_ptr<Animator> animator)
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

    Animator* GetAnimator(const PlayerMoveState animationName)
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

#endif //HEW_ANIMATOR_H