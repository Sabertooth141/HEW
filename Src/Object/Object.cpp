//
// Created by saber on 1/19/2026.
//

#include "Object.h"


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

void Object::Update(const float deltaTime, Tilemap& tileMap)
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

bool Object::CheckCollision(const Transform* other)
{
    const float targetLeft = other->topLeft.x;
    const float targetRight = other->topLeft.x + other->size.x;
    const float targetTop = other->topLeft.y;
    const float targetBottom = other->topLeft.y + other->size.y;

    const float selfLeft = transform.topLeft.x;
    const float selfRight = transform.topLeft.x + transform.size.x;
    const float selfTop = transform.topLeft.y;
    const float selfBottom = transform.topLeft.y + transform.size.y;

    return selfLeft < targetRight && selfRight > targetLeft && selfTop < targetBottom && selfBottom > targetTop;
}

bool Object::CheckCircleRectCollision(const float cx, const float cy, const float radius, const Transform* other)
{
    const float closestX = fmaxf(other->topLeft.x, fminf(cx, other->topLeft.x + other->size.x));
    const float closestY = fmaxf(other->topLeft.y, fminf(cy, other->topLeft.y + other->size.y));

    const float dx = cx - closestX;
    const float dy = cy - closestY;

    return dx * dx + dy * dy <= radius * radius;
}

float Object::Lerp(const float start, const float end, const float t)
{
    return start + (end - start) * t;
}

void Object::SetPosition(const float inX, const float inY)
{
    transform.topLeft.x = inX;
    transform.topLeft.y = inY;

    transform.CalculateCenterPosition();
}
