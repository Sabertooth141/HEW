//
// Created by saber on 1/19/2026.
//

#ifndef HEW_OBJECT_H
#define HEW_OBJECT_H

#include "../Lib/conioex.h"
#include "../World/Camera.h"
#include "../Config/Structs.h"
#include "../World/Tilemap.h"

class Object
{
public:
    Object();
    virtual ~Object() = default;

    virtual void Initialize(const ObjectConfig& config);
    virtual void Start();
    virtual void Update(float deltaTime, Tilemap& tileMap);
    virtual void Draw(Camera& cam);

    // OBJECT HELPER FUNCTIONS
    /**
     * collision between self(rect) and another rectangle
     * @param other transform of other
     * @return
     */
    virtual bool CheckCollision(const Transform* other);

    /**
     * collision between circle and another rectangle
     * @param cx circle center x
     * @param cy circle center y
     * @param radius circle radius
     * @param other rectangle transform
     * @return
     */
    virtual bool CheckCircleRectCollision(float cx, float cy, float radius, const Transform* other);

    virtual float Lerp(float start, float end, float t);

    virtual Vector2 GetPosition() { return transform.topLeft; }
    virtual Vector2 GetCenterPosition() { return transform.center; }
    virtual void SetPosition(float inX, float inY);

    virtual Vector2 GetSize() { return transform.size; }

public:
    Transform transform;

protected:
    // TEMP
    COLORS color;
};


#endif //HEW_OBJECT_H
