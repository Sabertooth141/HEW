//
// Created by saber on 1/23/2026.
//

#ifndef HEW_HITBOX_H
#define HEW_HITBOX_H

struct Hitbox
{
    float x, y;
    float offsetX, offsetY;
    float width, height;
    float lifeTime;
    float timer;
    bool isActive;

    void Activate(const float inX, const float inY, const float inOffsetX, const float inOffsetY,
                  const float inWidth, const float inHeight, const float inLifeTime)
    {
        x = inX + inOffsetX;
        y = inY + inOffsetY;
        offsetX = inOffsetX;
        offsetY = inOffsetY;

        width = inWidth;
        height = inHeight;
        lifeTime = inLifeTime;
        timer = 0;
        isActive = true;
    }

    void Update(const float deltaTime, const float inX, const float inY)
    {
        if (!isActive)
        {
            return;
        }

        x = inX + offsetX;
        y = inY + offsetY;

        timer += deltaTime;
        if (timer > lifeTime)
        {
            isActive = false;
        }
    }

    [[nodiscard]] bool CheckOverlap(const float otherX, const float otherY,
                                    const float otherW, const float otherH) const
    {
        if (!isActive)
        {
            return false;
        }

        return x < otherX + otherW &&
            x + width > otherX &&
            y < otherY + otherH &&
            y + height > otherY;
    }
};

#endif //HEW_HITBOX_H
