//
// Created by saber on 1/21/2026.
//

#ifndef HEW_SYSTEMCONFIGS_H
#define HEW_SYSTEMCONFIGS_H
#include "../VFX/VFXManager.h"

namespace sysCfg
{
    inline VFXConfig VFX()
    {
        VFXConfig config{};
        constexpr COLORREF normalPal[16] =
        {
            BLACK, /* #000000	黒				*/
            BLUE, /* #0000AA	青				*/
            GREEN, /* #00AA00	緑				*/
            CYAN, /* #00AAAA	シアン			*/
            RED, /* #AA0000	赤				*/
            MAGENTA, /* #AA00AA	マゼンタ		*/
            BROWN, /* #AA5500	茶				*/
            LIGHTGRAY, /* #AAAAAA	明るい灰色		*/
            DARKGRAY, /* #555555	暗い灰色		*/
            LIGHTBLUE, /* #5555FF	明るい青		*/
            LIGHTGREEN, /* #55FF55	明るい緑		*/
            LIGHTCYAN, /* #55FFFF	明るいシアン	*/
            LIGHTRED, /* #FF5555	明るい赤		*/
            LIGHTMAGENTA, /* #FF55FF	明るいマゼンタ	*/
            YELLOW, /* #FFFF55	黄				*/
            WHITE /* #FFFFFF	白				*/
        };

        config.normalPal = *normalPal;

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
        config.grayscale = *grayPal;

        return config;
    }
}

#endif //HEW_SYSTEMCONFIGS_H
