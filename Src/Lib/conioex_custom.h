//
// Created by saber on 2/4/2026.
//

#ifndef HEW_CONIOEX_CUSTOM_H
#define HEW_CONIOEX_CUSTOM_H
#include "conioex.h"

constexpr int bayer4x4[4][4] = {
    {0, 8, 2, 10},
    {12, 4, 14, 6},
    {3, 11, 1, 9},
    {15, 7, 13, 5}
};

void DrawPixelDithered(int x, int y, RGBQUAD color, float opacity);

void DrawCircleDithered(int cx, int cy, int r, int color, bool filled, float opacity);
void DrawCircleDithered(int cx, int cy, int r, RGBQUAD color, bool filled, float opacity);

void DrawLineHDithered(int x1, int y1, int x2, RGBQUAD rgb, float opacity);

template<typename T>
void Swap(T& _a, T& _b) { T tmp = _a; _a = _b; _b = tmp; }

void DebugPrintf(const char* format, ...);

int CalculateTextWidth(const wchar_t* text, int fontSize);

#endif //HEW_CONIOEX_CUSTOM_H
