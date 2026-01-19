//
// Created by saber on 1/19/2026.
//

#ifndef HEW_OBJECT_H
#define HEW_OBJECT_H


class Object
{
public:
    Object();
    virtual ~Object() = default;

    virtual float GetX() { return x;}
    virtual float GetY() { return y;}
    virtual float GetCenterX() {return x + width / 2; }
    virtual float GetCenterY() {return x + height / 2; }
    virtual void SetPosition(float inX, float inY);

    virtual int GetWidth() { return width; }
    virtual int GetHeight() { return height; }

protected:

    float x;
    float y;
    float width;
    float height;

};


#endif //HEW_OBJECT_H