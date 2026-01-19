//
// Created by saber on 1/19/2026.
//

#include "Object.h"

Object::Object() : x(0), y(0), width(0), height(0)
{
}

void Object::SetPosition(const float inX, const float inY)
{
    x = inX;
    y = inY;
}
