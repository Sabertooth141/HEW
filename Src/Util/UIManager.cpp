//
// Created by saber on 2/19/2026.
//

#include "UIManager.h"

#include "../Lib/conioex.h"

void UIManager::InitHPBar(const int x, const int y, const int width, const int height, Entity* target,
                          const bool isFollowTarget)
{
    HPBar hpBar{};

    hpBar.x = x;
    hpBar.y = y;
    hpBar.width = width;
    hpBar.height = height;
    hpBar.target = target;
    hpBar.isFollowTarget = isFollowTarget;

    hpBar.Initialize();

    hpBars.push_back(hpBar);
}

void UIManager::Initialize()
{
    timeRewindIndic = LoadBmp("../Assets/TutorialAssets/SkillIndicator1.bmp");
    timeStopIndic = LoadBmp("../Assets/TutorialAssets/SkillIndicator2.bmp");
    attackIndic = LoadBmp("../Assets/TutorialAssets/SkillIndicator3.bmp");
    movementIndic = LoadBmp("../Assets/TutorialAssets/SkillIndicator4.bmp");
}

void UIManager::Update(const float deltaTime)
{
    for (auto& hpBar : hpBars)
    {
        hpBar.Update(deltaTime);
    }
}

void UIManager::Draw() const
{
    if (Camera::Instance().GetLetterboxHeight() > 0)
    {
        return;
    }

    for (auto& hpBar : hpBars)
    {
        DrawHealthBar(hpBar);
    }
}

void UIManager::DrawHealthBar(const HPBar& bar)
{
    if (Camera::Instance().GetLetterboxHeight() > 0)
    {
        return;
    }

    const float maxHp = bar.target->GetMaxHp();

    if (maxHp <= 0) return;

    float ratio = bar.displayHp / maxHp;
    float trailRatio = bar.trailHp / maxHp;
    if (ratio < 0) ratio = 0;
    if (trailRatio < 0) trailRatio = 0;

    int fillWidth = static_cast<int>(bar.width * ratio);
    int trailWidth = static_cast<int>(bar.width * trailRatio);

    // Background
    DrawRect(bar.x, bar.y, bar.x + bar.width, bar.y + bar.height, DARKGRAY, true);

    // Trail
    if (trailWidth > 0)
    {
        DrawRect(bar.x, bar.y, bar.x + trailWidth, bar.y + bar.height, RED, true);
    }

    // Main fill
    int color = CYAN;
    if (ratio < 0.3f) color = LIGHTRED;
    else if (ratio < 0.6f) color = YELLOW;

    if (fillWidth > 0)
    {
        DrawRect(bar.x, bar.y, bar.x + fillWidth, bar.y + bar.height, color, true);
    }

    // Border
    DrawRect(bar.x, bar.y, bar.x + bar.width, bar.y + bar.height, WHITE, false);
}

void UIManager::DrawTimeRewindIndic(const int x, const int y) const
{
    if (Camera::Instance().GetLetterboxHeight() > 0)
    {
        return;
    }

    DrawBmp(x, y, timeRewindIndic);
}

void UIManager::DrawTimeStopIndic(const int x, const int y) const
{
    if (Camera::Instance().GetLetterboxHeight() > 0)
    {
        return;
    }

    DrawBmp(x, y, timeStopIndic);
}

void UIManager::DrawAttackIndic(const int x, const int y) const
{
    if (Camera::Instance().GetLetterboxHeight() > 0)
    {
        return;
    }

    DrawBmp(x, y, attackIndic);
}

void UIManager::DrawMovementIndic(const int x, const int y) const
{
    if (Camera::Instance().GetLetterboxHeight() > 0)
    {
        return;
    }

    DrawBmp(x, y, movementIndic);
}
