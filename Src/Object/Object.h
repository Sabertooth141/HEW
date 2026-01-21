//
// Created by saber on 1/19/2026.
//

#ifndef HEW_OBJECT_H
#define HEW_OBJECT_H

#include "../Lib/conioex.h"
#include "../World/Camera.h"

struct ObjectConfig {
    float x, y, width, height;
    COLORS color;
};

class Camera;
class Tilemap;

class Object
{
public:
    Object();
    virtual ~Object() = default;

    virtual void Initialize(const ObjectConfig& config);
    virtual void Start();
    virtual void Update(float deltaTime, const Tilemap& tileMap);
    virtual void Draw(const Camera& cam);

    virtual float GetX() { return x;}
    virtual float GetY() { return y;}
    virtual float GetCenterX() {return x + width / 2; }
    virtual float GetCenterY() {return y + height / 2; }
    virtual void SetPosition(float inX, float inY);

    virtual float GetWidth() { return width; }
    virtual float GetHeight() { return height; }

protected:

    float x;
    float y;
    float width;
    float height;

    // TEMP
    COLORS color;

};


#endif //HEW_OBJECT_H