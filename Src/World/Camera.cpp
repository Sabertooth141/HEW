//
// Created by saber on 1/15/2026.
//

#include "Camera.h"
#include "../Config/Structs.h"

Camera::Camera() : x(0.0f), y(0.0f),
                   viewW(800), viewH(600),
                   hasBounds(false),
                   minX(0.0f), minY(0.0f),
                   maxX(0.0f), maxY(0.0f),
                   followTarget()
{
}

void Camera::Initialize(const float startX, const float startY, const int viewWidth, const int viewHeight)
{
    x = startX;
    y = startY;
    viewW = viewWidth;
    viewH = viewHeight;
}

void Camera::SetPosition(const float inX, const float inY)
{
    x = inX;
    y = inY;

    if (hasBounds)
    {
        ClampToBounds();
    }
}

void Camera::Move(const float dx, const float dy)
{
    x += dx;
    y += dy;

    if (hasBounds)
    {
        ClampToBounds();
    }
}

void Camera::FollowTarget(const Vector2 inFollowTarget, const float smoothing)
{
    x += (inFollowTarget.x - x) * smoothing;
    y += (inFollowTarget.y - y) * smoothing;

    if (hasBounds)
    {
        ClampToBounds();
    }
}

void Camera::SetBounds(const float inMinX, const float inMinY, const float inMaxX, const float inMaxY)
{
    hasBounds = true;

    minX = inMinX;
    minY = inMinY;
    maxX = inMaxX;
    maxY = inMaxY;
    ClampToBounds();
}

void Camera::ClearBounds()
{
    hasBounds = false;
}

void Camera::ClampToBounds()
{
    if (!hasBounds)
    {
        return;
    }

    const float centerW = static_cast<float>(viewW) / 2.0f;
    const float centerH = static_cast<float>(viewH) / 2.0f;

    if (x - centerW < minX)
    {
        x = minX + centerW;
    }
    if (y - centerH < minY)
    {
        y = minY + centerH;
    }
    if (x + centerW > maxX)
    {
        x = maxX - centerW;
    }
    if (y + centerH > maxY)
    {
        y = maxY - centerH;
    }
}

void Camera::TriggerScreenShake(const float intensity, const float duration)
{
    shakeIntensity = intensity;
    shakeTimer = duration;
}

void Camera::UpdateShake(float deltaTime)
{
    if (shakeTimer <= 0)
    {
        shakeOffsetX = 0;
        shakeOffsetY = 0;
        return;
    }

    shakeTimer -= deltaTime;
    float t = shakeTimer > 0 ? shakeTimer / shakeIntensity : 0;

    shakeOffsetX = (rand() % 3 - 1) * shakeIntensity * t;
    shakeOffsetY = (rand() % 3 - 1) * shakeIntensity * t;
}

void Camera::UpdateLetterbox(const float deltaTime)
{
    letterboxMagnitude += (targetLetterbox - letterboxMagnitude) * letterboxSpeed * deltaTime;
}

float Camera::GetLeft() const
{
    return x - static_cast<float>(viewW) / 2.0f;
}

float Camera::GetRight() const
{
    return x + static_cast<float>(viewW) / 2.0f;
}

float Camera::GetTop() const
{
    return y - static_cast<float>(viewH) / 2.0f;
}

float Camera::GetBottom() const
{
    return y + static_cast<float>(viewH) / 2.0f;
}

int Camera::WorldToScreenX(const float worldX) const
{
    return static_cast<int>(worldX - GetLeft() + shakeOffsetX);
}

int Camera::WorldToScreenY(const float worldY) const
{
    return static_cast<int>(worldY - GetTop() + shakeOffsetY);
}

float Camera::ScreenToWorldX(const int screenX) const
{
    return static_cast<float>(screenX) + GetLeft();
}

float Camera::ScreenToWorldY(const int screenY) const
{
    return static_cast<float>(screenY) + GetTop();
}

bool Camera::IsVisible(const float worldX, const float worldY, const float width, const float height) const
{
    return !(worldX + static_cast<float>(width) < GetLeft() ||
        worldX > GetRight() ||
        worldY + static_cast<float>(height) < GetTop() ||
        worldY > GetBottom());
}
