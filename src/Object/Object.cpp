//
// Created by saber on 1/19/2026.
//

#include "Object.h"

#include "../lib/Shape.h"

Object::Object(const ObjectConfig& config) : x(config.x), y(config.y), width(config.width), height(config.y), color(config.color)
{
}

void Object::Start(const float inX, const float inY)
{
    x = inX;
    y = inY;
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

    DrawRect(static_cast<int>(x), static_cast<int>(y),
             static_cast<int>(x + width), static_cast<int>(y + height),
             color, true);
}

void Object::SetPosition(const float inX, const float inY)
{
    x = inX;
    y = inY;
}
