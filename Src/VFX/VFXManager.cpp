//
// Created by saber on 1/21/2026.
//

#include "VFXManager.h"

void VFXManager::Initialize(const VFXConfig& config)
{
    grayscalePal = config.grayscale;
    normalPal = config.normalPal;
}

void VFXManager::ApplyGrayscale()
{
    SetPalette16(&grayscalePal);
}

void VFXManager::ApplyNormalPal()
{
    SetPalette16(&normalPal);
}
