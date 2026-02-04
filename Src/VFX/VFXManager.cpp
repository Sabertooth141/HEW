//
// Created by saber on 1/21/2026.
//

#include "VFXManager.h"

extern RGBQUAD* g_FrameBuffer32bitD2D;
extern COORD g_ScreenBufferSize;

void VFXManager::Initialize(const VFXConfig& config)
{
    memcpy(normalPal, config.normalPal, sizeof(normalPal));
    memcpy(grayscalePal, config.grayscale, sizeof(grayscalePal));
}

void VFXManager::ApplyGrayscale() const
{
    SetPalette(grayscalePal, 0, 15);
}

void VFXManager::ApplyNormalPal() const
{
    SetPalette(normalPal, 0, 15);
}

void VFXManager::SetNormalPal(const COLORREF inPalette[16])
{
    memcpy(normalPal, inPalette, sizeof(normalPal));
}

void VFXManager::ApplyGrayScaleToFrameBuffer()
{
    if (g_FrameBuffer32bitD2D == nullptr)
        return;

    const int totalPixels = g_ScreenBufferSize.X * g_ScreenBufferSize.Y;

    for (int i = 0; i < totalPixels; i++)
    {
        RGBQUAD& pixel = g_FrameBuffer32bitD2D[i];

        // Standard luminance formula (ITU-R BT.601)
        // Gray = 0.299*R + 0.587*G + 0.114*B
        const BYTE gray = static_cast<BYTE>(
            0.299f * pixel.rgbRed +
            0.587f * pixel.rgbGreen +
            0.114f * pixel.rgbBlue
        );

        pixel.rgbRed = gray;
        pixel.rgbGreen = gray;
        pixel.rgbBlue = gray;
        // rgbReserved stays unchanged
    }
}
