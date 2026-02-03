//
// Created by saber on 2/3/2026.
//

#ifndef HEW_PHYSICS_H
#define HEW_PHYSICS_H
#include "../World/Tilemap.h"

struct RaycastHit
{
    bool hit;
    float hitX, hitY;
    float distance;
    float normalX, normalY;
};

struct Physics
{
    static Physics& Instance()
    {
        static Physics instance;
        return instance;
    }

    static RaycastHit Raycast(const Tilemap& tilemap, const float startX, const float startY, const float endX, const float endY)
    {
        RaycastHit hit{false, endX, endY, 0, 0, 0};
        const float dx = endX - startX;
        const float dy = endY - startY;
        const float distance = sqrtf(dx * dx + dy * dy);

        if (distance <= 0.01f)
        {
            return hit;
        }

        const float dirX = dx / distance;
        const float dirY = dy / distance;

        float stepSize = static_cast<float>(tilemap.GetTileSize()) * 0.5f;
        float currDistance = 0;

        while (currDistance < distance)
        {
            const float checkX = startX + dirX * currDistance;
            const float checkY = startY + dirY * currDistance;

            if (tilemap.IsSolidAt(checkX, checkY))
            {
                hit.hit = true;
                hit.distance = distance;
                hit.hitX = checkX - dirX * stepSize;
                hit.hitY = checkY - dirY * stepSize;

                if (tilemap.IsSolidAt(checkX - dirX * stepSize, checkY))
                    hit.normalY = (dirY > 0) ? -1 : 1;
                else
                    hit.normalX = (dirX > 0) ? -1 : 1;

                return hit;
            }
            currDistance += stepSize;
        }
        return hit;
    }
};


#endif //HEW_PHYSICS_H