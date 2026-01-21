//
// Created by saber on 1/21/2026.
//

#ifndef HEW_VFXMANAGER_H
#define HEW_VFXMANAGER_H
#include "../Object/Entity/Entity.h"

struct VFXConfig
{
    COLORREF normalPal;
    COLORREF grayscale;
};

class VFXManager
{
public:
    VFXManager() = default;

    void Initialize(const VFXConfig& config);

    void ApplyGrayscale();
    void ApplyNormalPal();

private:
    COLORREF grayscalePal{};
    COLORREF normalPal{};
    bool paletteSaved = false;
};


#endif //HEW_VFXMANAGER_H