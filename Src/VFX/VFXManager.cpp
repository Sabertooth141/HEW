//
// Created by saber on 1/21/2026.
//

#include "VFXManager.h"

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
