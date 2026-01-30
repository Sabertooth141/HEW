//
// Created by saber on 1/15/2026.
//

#ifndef HEW_CAMERA_H
#define HEW_CAMERA_H

struct Vector2;

class Camera
{
public:
    Camera();

    void Initialize(float startX, float startY, int viewWidth, int viewHeight);

    void SetPosition(float inX, float inY);
    void Move(float dx, float dy);
    void FollowTarget(Vector2 inFollowTarget, float smoothing = 0.1f);

    void SetBounds(float inMinX, float inMinY, float inMaxX, float inMaxY);
    void ClearBounds();
    void ClampToBounds();

    [[nodiscard]] float GetLeft() const;
    [[nodiscard]] float GetRight() const;
    [[nodiscard]] float GetTop() const;
    [[nodiscard]] float GetBottom() const;

    [[nodiscard]] float GetX() const { return x; }
    [[nodiscard]] float GetY() const { return y; }
    [[nodiscard]] int GetViewWidth() const { return viewW; }
    [[nodiscard]] int GetViewHeight() const { return viewH; }

    [[nodiscard]] int WorldToScreenX(float worldX) const;
    [[nodiscard]] int WorldToScreenY(float worldY) const;
    [[nodiscard]] float ScreenToWorldX(int screenX) const;
    [[nodiscard]] float ScreenToWorldY(int screenY) const;

    [[nodiscard]] bool IsVisible(float worldX, float worldY, float width, float height) const;

private:
    float x, y;

    int viewW, viewH;

    bool hasBounds;
    float minX, minY, maxX, maxY;

    Vector2* followTarget;
};


#endif //HEW_CAMERA_H
