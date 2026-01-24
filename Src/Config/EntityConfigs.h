//
// Created by saber on 1/21/2026.
//

#pragma once
#ifndef HEW_ENTITYCONFIGS_H
#define HEW_ENTITYCONFIGS_H
#include "../Object/Entity/Enemy/Enemy.h"
#include "../Object/Entity/Player/PlayerController.h"

namespace config
{
    inline PlayerConfig Player()
    {
        PlayerConfig config{};
        config.width = 16;
        config.height = 24;
        config.color = LIGHTCYAN;
        config.walkSpeed = 200.0f;
        config.sprintSpeed = 400.0f;
        config.jumpForce = 400.0f;
        config.gravity = 800.0f;
        config.maxFallSpeed = 1000.0f;

        config.maxHp = 100;

        config.isFacingRight = true;
        config.timeStopDuration = 3.0f;

        config.damage = 20;
        config.attackCooldown = 1;

        config.attackOffsetX = 3;
        config.attackOffsetY = 0;
        config.attackHeight = 20;
        config.attackWidth = 15;

        config.attackDuration = .2;

        return config;
    }

    inline EnemyConfig Enemy()
    {
        EnemyConfig config{};
        config.width = 16;
        config.height = 16;
        config.color = RED;
        config.gravity = 800.0f;
        config.maxFallSpeed = 1000.0f;

        config.maxHp = 100;
        config.isFacingRight = true;

        config.moveSpeed = 200.0f;
        config.attackCooldown = 2;

        return config;
    }
}

#endif //HEW_ENTITYCONFIGS_H
