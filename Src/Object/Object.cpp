//
// Created by saber on 1/19/2026.
//

#include "Object.h"

#include "../Lib/Shape.h"


Object::Object() : x(0), y(0), width(0), height(0), color()
{
}

void Object::Initialize(const ObjectConfig& config)
{
    x = config.x;
    y = config.y;
    width = config.width;
    height = config.height;
    color = config.color;
}

void Object::Start()
{
}

void Object::Update(float deltaTime, const Tilemap& tileMap)
{
}

void Object::Draw(const Camera& cam)
{
    if (!cam.IsVisible(x, y, width, height))
    {
        return;
    }

    const int screenX = cam.WorldToScreenX(x);
    const int screenY = cam.WorldToScreenY(y);

    DrawRect(screenX, screenY,
             screenX + width, screenY + height,
             color, true);
}

void Object::SetPosition(const float inX, const float inY)
{
    x = inX;
    y = inY;
}
