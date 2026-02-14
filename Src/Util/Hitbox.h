//
// Created by saber on 1/23/2026.
//

#ifndef HEW_HITBOX_H
#define HEW_HITBOX_H
#include "../Config/Structs.h"

struct Hitbox
{
    Transform transform;
    float offsetX, offsetY;
    float lifeTime;
    float timer;
    bool isActive;

    void Activate(const float inX, const float inY, const float inOffsetX, const float inOffsetY,
                  const float inWidth, const float inHeight, const float inLifeTime)
    {
        transform.center.x =  inX + inOffsetX;
        transform.center.y = inY + inOffsetY;
        offsetX = inOffsetX;
        offsetY = inOffsetY;

        transform.size.x = inWidth;
        transform.size.y = inHeight;
        lifeTime = inLifeTime;
        timer = 0;
        isActive = true;

        transform.CalculateTopLeftPosition();
    }

    void Update(const float deltaTime, const float inX, const float inY)
    {
        if (!isActive)
        {
            return;
        }

        transform.center.x = inX + offsetX;
        transform.center.y = inY + offsetY;

        transform.CalculateTopLeftPosition();

        if (lifeTime == -1)
        {
            return;
        }

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

        return transform.topLeft.x < otherX + otherW &&
            transform.topLeft.x + transform.size.x > otherX &&
            transform.topLeft.y < otherY + otherH &&
            transform.topLeft.y + transform.size.y > otherY;
    }
};

#endif //HEW_HITBOX_H
