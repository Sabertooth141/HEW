//
// Created by saber on 2/4/2026.

#include "conioex_custom.h"

/**
 *
 * @param x pixel X coord
 * @param y pixel Y coord
 * @param color pixel color
 * @param opacity pixel opacity (0 - 1)
 */
void DrawPixelDithered(const int x, const int y, const RGBQUAD color, const float opacity)
{
    const int threshold = bayer4x4[y % 4][x % 4];
    const int level = static_cast<int>(opacity * 16.0);

    if (level > threshold)
    {
        DrawPixel(x, y, color);
    }
}

void DrawCircleDithered(const int cx, const int cy, const int r, const int color, const bool filled,
                        const float opacity)
{
    DrawCircleDithered(cx, cy, r, g_PaletteD2D[color % NUM_D2D_PAL], filled, opacity);
}

void DrawCircleDithered(const int cx, const int cy, const int r, const RGBQUAD color, const bool filled,
                        const float opacity)
{
    int D = r;
    int x = (D - 1);
    int y = 0;
    if (filled)
    {
        while (x >= y)
        {
            //右回りで水平が０度：１回で８ドット（８象限分）を描く
            DrawLineHDithered(cx, cy + y, cx + x,/* _cy + y,*/ color, opacity); //第１象限：　　０°?
            DrawLineHDithered(cx, cy + x, cx + y,/* _cy + x,*/ color, opacity); //第２象限：　４５°?
            DrawLineHDithered(cx, cy + x, cx - y,/* _cy + x,*/ color, opacity); //第３象限：　９０°?
            DrawLineHDithered(cx, cy + y, cx - x,/* _cy + y,*/ color, opacity); //第３象限：１３５°?
            DrawLineHDithered(cx, cy - y, cx - x,/* _cy - y,*/ color, opacity); //第３象限：１８０°?
            DrawLineHDithered(cx, cy - x, cx - y,/* _cy - x,*/ color, opacity); //第３象限：２２５°?
            DrawLineHDithered(cx, cy - x, cx + y,/* _cy - x,*/ color, opacity); //第３象限：２７０°?
            DrawLineHDithered(cx, cy - y, cx + x,/* _cy - y,*/ color, opacity); //第３象限：３１５°?３６０°
            D -= (y << 1);
            if (D <= 0)
            {
                x--;
                D += (x << 1);
            }
            y++;
        }
    }
    else
    {
        while (x >= y)
        {
            //右回りで水平が０度：１回で８ドット（８象限分）を描く
            DrawPixelDithered(cx + x, cy + y, color, opacity); //第１象限：　　０°?
            DrawPixelDithered(cx + y, cy + x, color, opacity); //第２象限：　４５°?
            DrawPixelDithered(cx - y, cy + x, color, opacity); //第３象限：　９０°?
            DrawPixelDithered(cx - x, cy + y, color, opacity); //第３象限：１３５°?
            DrawPixelDithered(cx - x, cy - y, color, opacity); //第３象限：１８０°?
            DrawPixelDithered(cx - y, cy - x, color, opacity); //第３象限：２２５°?
            DrawPixelDithered(cx + y, cy - x, color, opacity); //第３象限：２７０°?
            DrawPixelDithered(cx + x, cy - y, color, opacity); //第３象限：３１５°?３６０°
            D -= (y << 1);
            if (D <= 0)
            {
                x--;
                D += (x << 1);
            }
            y++;
        }
    }
} //DDA_Circle

void DrawLineHDithered(int x1, int y1, int x2, RGBQUAD rgb, const float opacity)
{
    if (x2 < x1)
    {
        //left<Rightにする
        Swap(x1, x2);
    }
    for (; x1 <= x2; x1++)
    {
        DrawPixelDithered(x1, y1, rgb, opacity);
    }
}

void DebugPrintf(const char* format, ...)
{
    char buffer[512];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    OutputDebugStringA(buffer);
}
