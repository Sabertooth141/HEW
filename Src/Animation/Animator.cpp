//
// Created by saber on 1/28/2026.
//

#include "Animator.h"

#include "SpriteSheetLoader.h"

Animator::Animator() : spriteSheet(nullptr),
                       currFrame(0),
                       startFrame(0),
                       endFrame(0),
                       frameTimer(0),
                       isPlaying(false),
                       isPaused(false),
                       isLoop(true),
                       isDirectionForward(true)
{
}

Animator::~Animator()
{
    if (spriteSheet != nullptr)
    {
        delete spriteSheet;
        spriteSheet = nullptr;
    }
}

bool Animator::LoadSpriteSheet(const char* jsonFile, const char* bmpFile)
{
    if (spriteSheet != nullptr)
    {
        delete spriteSheet;
    }

    spriteSheet = SpriteSheetLoader::LoadFromFile(jsonFile, bmpFile);
    return spriteSheet != nullptr && !spriteSheet->frames.empty();
}

void Animator::Play(const bool loop)
{
    if (spriteSheet == nullptr)
    {
        return;
    }

    PlayFromFrame(0, loop);

    if (isDirectionForward)
    {
        currFrame = startFrame;
    }
    else
    {
        currFrame = endFrame;
    }
    isPlaying = true;
    isPaused = false;
    isLoop = loop;
}

void Animator::PlayFromFrame(const int inStartFrame, const int inEndFrame, const bool loop)
{
    if (spriteSheet == nullptr)
    {
        return;
    }

    if (spriteSheet->frames.empty())
    {
        return;
    }

    startFrame = inStartFrame;
    endFrame = inEndFrame;

    currFrame = inStartFrame;
    frameTimer = 0;
    isPlaying = true;
    isPaused = false;
    isLoop = loop;
    isDirectionForward = true;
}

void Animator::Stop()
{
    isPlaying = false;
    currFrame = startFrame;
    frameTimer = 0;
}

void Animator::Pause()
{
    isPaused = true;
}

void Animator::Resume()
{
    isPlaying = false;
}

void Animator::Update(const float deltaTime)
{
    if (spriteSheet == nullptr)
    {
        return;
    }

    if (!isPlaying || !isPaused)
    {
        return;
    }

    frameTimer += deltaTime;

    const SpriteFrame& frame = spriteSheet->frames[currFrame];

    if (frameTimer >= frame.duration)
    {
        frameTimer -= frame.duration;
        AdvanceFrame();
    }
}

void Animator::Draw(const Camera& cam, const float worldX, const float worldY, const bool flipHorizontal) const
{
    const SpriteFrame* frame = GetCurrentFrame();

    if (frame == nullptr || frame->image == nullptr)
    {
        return;
    }

    if (!cam.IsVisible(worldX, worldY, static_cast<float>(GetFrameWidth()), static_cast<float>(GetFrameHeight())))
    {
        return;
    }

    const int screenX = cam.WorldToScreenX(worldX);
    const int screenY = cam.WorldToScreenY(worldY);

    if (flipHorizontal)
    {
        // TODO: implement flipped draw BMP
        DrawBmp(screenX, screenY, frame->image, true);
    }
    else
    {
        DrawBmp(screenX, screenY, frame->image, false);
    }

}

SpriteFrame* Animator::GetCurrentFrame() const
{
    if (spriteSheet == nullptr || spriteSheet->frames.empty())
    {
        return nullptr;
    }

    if (currFrame < 0 || currFrame > static_cast<int>(spriteSheet->frames.size() - 1))
    {
        return nullptr;
    }

    return &spriteSheet->frames[currFrame];
}

int Animator::GetFrameWidth() const
{
    const SpriteFrame* frame = GetCurrentFrame();
    return frame ? frame->w : 0;
}

int Animator::GetFrameHeight() const
{
    const SpriteFrame* frame = GetCurrentFrame();
    return frame ? frame->h : 0;
}

void Animator::AdvanceFrame()
{
    if (isDirectionForward)
    {
        currFrame++;
        if (currFrame > endFrame)
        {
            if (isLoop)
            {
                currFrame = startFrame;
            }
            else
            {
                currFrame = endFrame;
                isPlaying = false;
            }
        }
    }
    else
    {
        currFrame--;
        if (currFrame < startFrame)
        {
            if (isLoop)
            {
                currFrame = endFrame;
            }
            else
            {
                currFrame = startFrame;
                isPlaying = false;
            }
        }
    }
}
