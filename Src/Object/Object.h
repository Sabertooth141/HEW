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
    virtual void Update(float deltaTime, const Tilemap& tileMap);
    virtual void Draw(const Camera& cam);

    virtual Vector2 GetPosition() { return transform.topLeft;}
    virtual Vector2 GetCenterPosition() {return transform.center; }
    virtual void SetPosition(float inX, float inY);

    virtual Vector2 GetSize() { return transform.size; }

public:
    Transform transform;

protected:
    // TEMP
    COLORS color;

};


#endif //HEW_OBJECT_H