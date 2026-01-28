//
// Created by saber on 1/28/2026.
//

#ifndef HEW_SPRITEFRAME_H
#define HEW_SPRITEFRAME_H
#include <vector>

#include "../Lib/conioex.h"

struct SpriteFrame
{
    int x, y; // position in spritesheet
    int w, h; // size
    float duration; // frame duration in SEC
    Bmp* image;

    SpriteFrame() : x(0), y(0), w(0), h(0), duration(0), image(nullptr) {}
};

struct SpriteSheet
{
    Bmp* source;
    std::vector<SpriteFrame> frames;
    int imageWidth, imageHeight;

    SpriteSheet() : source(nullptr), imageWidth(0), imageHeight(0) {}

    ~SpriteSheet()
    {
        for (SpriteFrame &frame : frames)
        {
            if (frame.image != nullptr)
            {
                DeleteBmp(&frame.image);
            }
        }
        frames.clear();

        if (source != nullptr)
        {
            DeleteBmp(&source);
            source = nullptr;
        }
    }
};

#endif //HEW_SPRITEFRAME_H