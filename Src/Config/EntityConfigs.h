//
// Created by saber on 1/21/2026.
//

#pragma once
#ifndef HEW_ENTITYCONFIGS_H
#define HEW_ENTITYCONFIGS_H
#include "../Config/Structs.h"

namespace config
{
    inline PlayerConfig Player()
    {
        PlayerConfig config{};
        config.width = 64;
        config.height = 64;
        config.color = LIGHTCYAN;
        config.walkSpeed = 300.0f;
        config.dashSpeed = 1000.0f;
        config.jumpForce = 500.0f;
        config.gravity = 1000.0f;
        config.maxFallSpeed = 1000.0f;
        config.airResistance = 3.0f;
        config.dashDuration = 0.1f;

        config.maxHp = 100;

        config.isFacingRight = true;
        config.timeStopDuration = 3.0f;

        return config;
    }

    inline PlayerAttackConfig PlayerAttack()
    {
        PlayerAttackConfig config(3);

        // attk 0
        config.data[0].damage = 10;
        config.data[0].offsetX = 30;
        config.data[0].offsetY = 0;
        config.data[0].width = 60;
        config.data[0].height = 54;
        config.data[0].duration = 1;    // to be set by frame data IN SEC
        config.data[0].recovery = 0.5f;

        // attk 1
        config.data[1].damage = 20;
        config.data[1].offsetX = 50;
        config.data[1].offsetY = 0;
        config.data[1].width = 80;
        config.data[1].height = 54;
        config.data[1].duration = 1;    // to be set by frame data IN SEC
        config.data[1].recovery = 0.8f;

        // attk 2
        config.data[2].damage = 40;
        config.data[2].offsetX = 40;
        config.data[2].offsetY = 0;
        config.data[2].width = 60;
        config.data[2].height = 54;
        config.data[2].duration = 1;    // to be set by frame data IN SEC
        config.data[2].recovery = 0.3f;

        return config;
    }

    inline EnemyConfig Enemy()
    {
        EnemyConfig config{};
        config.width = 64;
        config.height = 64;
        config.color = GREEN;
        config.gravity = 800.0f;
        config.maxFallSpeed = 1000.0f;
        config.checkGroundOffset = 2;

        config.maxHp = 100;
        config.isFacingRight = true;

        config.moveSpeed = 200.0f;
        config.attackCooldown = 2;

        config.invicCooldown = 0.5;

        return config;
    }

    inline MineConfig Mine()
    {
        MineConfig config{};
        config.width = 32;
        config.height = 32;
        config.color = GREEN;
        config.gravity = 800.0f;
        config.maxFallSpeed = 1000.0f;
        config.checkGroundOffset = 2;

        config.maxHp = 50;
        config.isFacingRight = true;

        config.moveSpeed = 200.0f;

        config.explosionRadius = 100;
        config.timeToExplode = 1.5;

        config.invicCooldown = 0.5;

        return config;
    }
}

#endif //HEW_ENTITYCONFIGS_H
