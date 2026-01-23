//
// Created by saber on 1/21/2026.
//

#ifndef HEW_SYSTEMCONFIGS_H
#define HEW_SYSTEMCONFIGS_H
#include "../Systems/TimeManager.h"
#include "../VFX/VFXManager.h"

namespace sysCfg
{
    inline VFXConfig VFXCfg()
    {
        VFXConfig config{};

        constexpr COLORREF normalPal[16] =
        {
            0x00000000,  // BLACK        #000000
            0x00AA0000,  // BLUE         #0000AA
            0x0000AA00,  // GREEN        #00AA00
            0x00AAAA00,  // CYAN         #00AAAA
            0x000000AA,  // RED          #AA0000
            0x00AA00AA,  // MAGENTA      #AA00AA
            0x000055AA,  // BROWN        #AA5500
            0x00AAAAAA,  // LIGHTGRAY    #AAAAAA
            0x00555555,  // DARKGRAY     #555555
            0x00FF5555,  // LIGHTBLUE    #5555FF
            0x0055FF55,  // LIGHTGREEN   #55FF55
            0x00FFFF55,  // LIGHTCYAN    #55FFFF
            0x005555FF,  // LIGHTRED     #FF5555
            0x00FF55FF,  // LIGHTMAGENTA #FF55FF
            0x0055FFFF,  // YELLOW       #FFFF55
            0x00FFFFFF   // WHITE        #FFFFFF
        };

        constexpr COLORREF grayPal[16] =
        {
            0x000000, // BLACK
            0x1A1A1A, // BLUE -> dark gray
            0x2A2A2A, // GREEN -> dark gray
            0x3A3A3A, // CYAN
            0x2A2A2A, // RED
            0x3A3A3A, // MAGENTA
            0x4A4A4A, // BROWN
            0x8A8A8A, // LIGHTGRAY
            0x4A4A4A, // DARKGRAY
            0x6A6A6A, // LIGHTBLUE
            0x7A7A7A, // LIGHTGREEN
            0x9A9A9A, // LIGHTCYAN
            0x6A6A6A, // LIGHTRED
            0x9A9A9A, // LIGHTMAGENTA
            0xCACACA, // YELLOW
            0xFFFFFF // WHITE
        };

        memcpy(config.normalPal, normalPal, sizeof(normalPal));
        memcpy(config.grayscale, grayPal, sizeof(grayPal));

        return config;
    }

    inline TimeManagerConfig TimeCfg()
    {
        TimeManagerConfig config{};

        // time stop
        config.timeStopCooldownMax = 2;

        // time rewind
        config.rewindCooldownMax = 2;
        config.rewindMagnitude = 0.5;

        return config;
    }
}

#endif //HEW_SYSTEMCONFIGS_H
