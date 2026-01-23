//
// Created by saber on 1/21/2026.
//

#ifndef HEW_VFXMANAGER_H
#define HEW_VFXMANAGER_H
#include "../Object/Entity/Entity.h"

struct VFXConfig
{
    COLORREF normalPal[16];
    COLORREF grayscale[16];
};

class VFXManager
{
public:
    VFXManager() = default;

    void Initialize(const VFXConfig& config);

    void ApplyGrayscale() const;
    void ApplyNormalPal() const;

private:
    COLORREF grayscalePal[16]{};
    COLORREF normalPal[16]{};
    bool paletteSaved = false;
};


#endif //HEW_VFXMANAGER_H