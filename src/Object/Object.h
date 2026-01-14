//
// Created by saber on 1/14/2026.
//

#ifndef HEW_OBJECT_H
#define HEW_OBJECT_H

struct Transform
{
    float x;
    float y;
};

class Object
{
    // function
public:
    virtual ~Object() = default;
    virtual void Start();
    virtual void Update();
    virtual void Draw();

    // vars
public:
    Transform transform{};

};


#endif //HEW_OBJECT_H
