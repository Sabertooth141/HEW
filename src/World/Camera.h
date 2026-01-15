//
// Created by saber on 1/15/2026.
//

#ifndef HEW_CAMERA_H
#define HEW_CAMERA_H


class Camera
{
public:
    Camera(int inX, int inY, int inWidth, int inHeight);

private:
    int x = 0;
    int y = 0;

    int viewW = 0;
    int viewH = 0;
};


#endif //HEW_CAMERA_H
