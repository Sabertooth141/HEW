//
// Created by saber on 1/21/2026.
//

#pragma once
#ifndef HEW_ENTITYCONFIGS_H
#define HEW_ENTITYCONFIGS_H
#include "../Object/Entity/Player/PlayerController.h"

namespace config
{
    inline PlayerConfig Player()
    {
        PlayerConfig config{};
        config.x = 0;
        config.y = 0;
        config.width = 16;
        config.height = 24;
        config.color = LIGHTCYAN;
        config.walkSpeed = 200.0f;
        config.sprintSpeed = 400.0f;
        config.jumpForce = 400.0f;
        config.gravity = 800.0f;
        config.maxFallSpeed = 1000.0f;
        config.currHp = 100;
        config.maxHp = 100;

        config.velX = 0;
        config.velY = 0;
        config.isFacingRight = true;
        config.timeStopDuration = 3.0f;

        return config;
    }
}

#endif //HEW_ENTITYCONFIGS_H
