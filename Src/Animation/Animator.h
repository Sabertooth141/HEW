//
// Created by saber on 1/28/2026.
//

#ifndef HEW_ANIMATOR_H
#define HEW_ANIMATOR_H

#include "SpriteFrame.h"
#include "../World/Camera.h"

enum class PlayerAnimations
{
    MOVE,
    IDLE,
    ATTK1,
    ATTK2,
    ATTK3
};

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
    int endFrame;

    float frameTimer;
    bool isPlaying;
    bool isPaused;
    bool isLoop;

    bool isDirectionForward;
};

#endif //HEW_ANIMATOR_H