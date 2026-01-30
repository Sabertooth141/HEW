//
// Created by saber on 1/28/2026.
//

#ifndef HEW_SPRITESHEETLOADER_H
#define HEW_SPRITESHEETLOADER_H
#include <string>

#include "SpriteFrame.h"


class SpriteSheetLoader
{
public:
    // loads sprite sheet from bmp(4 bit) and json file(frame info)
    static SpriteSheet* LoadFromFile(const char* jsonPath, const char* bmpPath);

private:
    static bool ParseJson(const char* jsonPath, SpriteSheet* spriteSheet);
    static void ExtractFrames(SpriteSheet* spriteSheet);
    static Bmp* ExtractRegion(const Bmp* source, int x, int y, int width, int height);

    // json helpers
    static std::string ReadFile(const char* path);
    static int ParseInt(const std::string& json, const std::string& key);
};


#endif //HEW_SPRITESHEETLOADER_H