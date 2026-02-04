//
// Created by saber on 1/21/2026.
//

#ifndef HEW_VFXMANAGER_H
#define HEW_VFXMANAGER_H

#include "../Lib/conioex.h"
#include "../Config/Structs.h"

class VFXManager
{
public:
    VFXManager() = default;

    void Initialize(const VFXConfig& config);

    void ApplyGrayscale() const;
    void ApplyNormalPal() const;
    void SetNormalPal(const COLORREF inPalette[16]);

    void ApplyGrayScaleToFrameBuffer();

private:
    COLORREF grayscalePal[16]{};
    COLORREF normalPal[16]{};
    bool paletteSaved = false;
};


#endif //HEW_VFXMANAGER_H