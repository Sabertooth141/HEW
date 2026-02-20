//
// Created by saber on 2/19/2026.
//

#ifndef HEW_UIMANAGER_H
#define HEW_UIMANAGER_H
#include "../Object/Entity/Entity.h"

struct HPBar
{
    float displayHp = 0;
    float trailHp = 0;
    float trailDelay = 0;
    bool isFollowTarget = false;

    int x, y;
    int width, height;

    Entity* target = nullptr;

    static constexpr float DRAIN_SPEED = 80.0f;
    static constexpr float TRAIL_DELAY = 0.4f;
    static constexpr float TRAIL_SPEED = 50.0f;

    void Initialize()
    {
        displayHp = target->GetMaxHp();
        trailHp = target->GetMaxHp();
    }

    void Update(const float deltaTime)
    {
        float actualHp = target->GetCurrentHp();

        if (displayHp < actualHp)
        {
            displayHp += DRAIN_SPEED * deltaTime * 2;

            if (displayHp > actualHp)
            {
                displayHp = actualHp;
            }

            trailHp = actualHp;
        }

        if (displayHp > actualHp)
        {
            displayHp -= DRAIN_SPEED * deltaTime;
            if (displayHp < actualHp)
            {
                displayHp = actualHp;
            }
            trailDelay = TRAIL_DELAY;
        }

        if (trailHp > displayHp)
        {
            trailDelay -= deltaTime;
            if (trailDelay <= 0)
            {
                trailHp -= TRAIL_SPEED * deltaTime;
                if (trailHp < displayHp)
                {
                    trailHp = displayHp;
                }
            }
        }
    }
};

class UIManager
{
public:
    static UIManager& Instance()
    {
        static UIManager instance;
        return instance;
    }

    void InitHPBar(int x, int y, int width, int height, Entity* target, bool isFollowTarget = false);
    void Initialize();
    void Update(float deltaTime);
    void Draw() const;

    static void DrawHealthBar(const HPBar& bar);

    void DrawTimeRewindIndic(int x, int y) const;
    void DrawTimeStopIndic(int x, int y) const;
    void DrawAttackIndic(int x, int y) const;
    void DrawMovementIndic(int x, int y) const;

private:
    std::vector<HPBar> hpBars;

    Bmp* timeRewindIndic = nullptr;
    Bmp* timeStopIndic = nullptr;
    Bmp* attackIndic = nullptr;
    Bmp* movementIndic = nullptr;
};


#endif //HEW_UIMANAGER_H
