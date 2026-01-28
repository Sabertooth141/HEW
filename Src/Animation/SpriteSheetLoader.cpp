//
// Created by saber on 1/28/2026.
//

#include "SpriteSheetLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>

SpriteSheet* SpriteSheetLoader::LoadFromFile(const char* bmpPath, const char* jsonPath)
{
    auto* sheet = new SpriteSheet();

    sheet->source = LoadBmp(bmpPath, true);
    if (sheet->source == nullptr)
    {
        delete sheet;
        return nullptr;
    }

    if (!ParseJson(jsonPath, sheet))
    {
        delete sheet;
        return nullptr;
    }

    ExtractFrames(sheet);

    return sheet;
}

bool SpriteSheetLoader::ParseJson(const char* jsonPath, SpriteSheet* spriteSheet)
{
    std::string info = ReadFile(jsonPath);
    if (info.empty())
    {
        return false;
    }

    size_t pos = info.find("\"frames\"");
    if (pos == std::string::npos)
    {
        return false;
    }

    while (true)
    {
        // find: "frame": {x, y, w, h}
        const size_t framePos = info.find("\"frame\":", pos + 1);
        if (framePos == std::string::npos)
        {
            break;
        }

        const size_t frameInfoStart = info.find('{', framePos);
        const size_t frameInfoEnd = info.find('}', frameInfoStart);
        if (frameInfoStart == std::string::npos || frameInfoEnd == std::string::npos)
        {
            break;
        }

        std::string frameInfo = info.substr(frameInfoStart, frameInfoEnd - frameInfoStart + 1);

        SpriteFrame frame;
        frame.x = ParseInt(frameInfo, "x");
        frame.y = ParseInt(frameInfo, "y");
        frame.w = ParseInt(frameInfo, "w");
        frame.h = ParseInt(frameInfo, "h");

        const size_t durationPos = info.find("\"duration\":", frameInfoEnd);
        const size_t nextFramePos = info.find("\"frame\":", frameInfoEnd);

        // make sure getting duration from this frame | if not set to default value (0.1 sec)
        if (durationPos != std::string::npos && (nextFramePos == std::string::npos || durationPos < nextFramePos))
        {
            const int durationMs = ParseInt(info.substr(durationPos, 30), "duration");
            frame.duration = static_cast<float>(durationMs) / 1000.0f;
        }
        else
        {
            frame.duration = 0.1f;
        }

        spriteSheet->frames.push_back(frame);
        pos = nextFramePos;
    }

    return !spriteSheet->frames.empty();
}

void SpriteSheetLoader::ExtractFrames(SpriteSheet* spriteSheet)
{
    if (spriteSheet->source == nullptr)
    {
        return;
    }

    for (auto& frame : spriteSheet->frames)
    {
        frame.image = ExtractRegion(spriteSheet->source, frame.x, frame.y, frame.w, frame.h);
    }
}

Bmp* SpriteSheetLoader::ExtractRegion(const Bmp* source, const int x, const int y, const int width, const int height)
{
    if (source->pixel == nullptr)
    {
        return nullptr;
    }

    const int bytesPerPixel = source->colbit <= 8 ? 1 : source->colbit / 8;

    auto* region = static_cast<Bmp*>(calloc(1, sizeof(Bmp)));

    if (region == nullptr)
    {
        return nullptr;
    }

    region->width = width;
    region->height = height;
    region->colbit = source->colbit;
    region->numpal = source->numpal;
    region->numpix = width * height * bytesPerPixel;
    region->swapRB = source->swapRB;

    if (source->pal != nullptr && source->numpal > 0)
    {
        region->pal = static_cast<COLORREF*>(calloc(source->numpal, sizeof(COLORREF)));
        if (region->pal != nullptr)
        {
            memcpy(region->pal, source->pal, source->numpal * sizeof(COLORREF));
        }
    }

    region->pixel = static_cast<char*>(calloc(region->numpix, sizeof(char)));
    if (region->pixel == nullptr)
    {
        DeleteBmp(&region);
        return nullptr;
    }

    const int srcStride = source->width * bytesPerPixel;

    for (int row = 0; row < height; row++)
    {
        const int srcIndex = (y + row) * srcStride + x * bytesPerPixel;
        const int destIndex = row * y * bytesPerPixel;

        memcpy(&region->pixel[destIndex], &source->pixel[srcIndex], width * bytesPerPixel);
    }

    return region;
}

std::string SpriteSheetLoader::ReadFile(const char* path)
{
    const std::ifstream file(path);
    if (!file.is_open())
    {
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

int SpriteSheetLoader::ParseInt(const std::string& json, const std::string& key)
{
    const std::string searchKey = "\"" + key + "\"";
    size_t pos = json.find(searchKey);
    if (pos == std::string::npos)
    {
        return 0;
    }

    pos += searchKey.length();

    while (pos < json.length() && (json[pos] == ' ' || json[pos] == '\t'))
    {
        pos++;
    }

    const char* valFound = json.c_str() + pos;
    char* endPtr;
    errno = 0;

    const auto result = static_cast<int>(std::strtol(valFound, &endPtr, 10));

    if (endPtr == valFound)
    {
        std::cerr << "Failed to parse int: " << valFound << ", no digits found" << std::endl;
        return 0;
    }
    if (errno == ERANGE)
    {
        std::cerr << "Failed to parse int: " << valFound << ", value out of range for a long int" << std::endl;
        return 0;
    }

    return result;
}
