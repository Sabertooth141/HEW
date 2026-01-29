//
// Created by saber on 1/19/2026.
//

#include "Object.h"

#include "../Lib/Shape.h"


Object::Object() : transform(), color()
{
}

void Object::Initialize(const ObjectConfig& config)
{
    transform.topLeft.x = config.x;
    transform.topLeft.y = config.y;
    transform.size.x = config.width;
    transform.size.y = config.height;
    color = config.color;

    transform.CalculateCenterPosition();
}

void Object::Start()
{
}

void Object::Update(float deltaTime, const Tilemap& tileMap)
{
}

void Object::Draw(const Camera& cam)
{
    if (!cam.IsVisible(transform.topLeft.x, transform.topLeft.y, transform.size.x, transform.size.y))
    {
        return;
    }

    const int screenX = cam.WorldToScreenX(transform.topLeft.x);
    const int screenY = cam.WorldToScreenY(transform.topLeft.y);

    DrawRect(screenX, screenY,
             screenX + transform.size.x, screenY + transform.size.y,
             color, true);
}

void Object::SetPosition(const float inX, const float inY)
{
    transform.topLeft.x = inX;
    transform.topLeft.y = inY;

    transform.CalculateCenterPosition();
}
