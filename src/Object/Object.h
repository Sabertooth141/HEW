//
// Created by saber on 1/14/2026.
//

#ifndef HEW_OBJECT_H
#define HEW_OBJECT_H


class Object
{
public:
    virtual ~Object() = default;
    virtual void Start();
    virtual void Update();
    virtual void Draw();
};


#endif //HEW_OBJECT_H
