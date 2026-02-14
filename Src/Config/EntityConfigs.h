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
        config.checkGroundOffset = 10;

        config.trailFadeDuration = 0.5;

        config.maxHp = 100;

        config.isFacingRight = true;
        config.timeStopDuration = 3.0f;

        config.knockBackRecoveryTime = 1.2;

        return config;
    }

    inline PlayerAttackConfig PlayerAttack()
    {
        PlayerAttackConfig config(4);

        // attk 0
        config.data[0].damage = 10;
        config.data[0].hitboxOffsetX = 40;
        config.data[0].hitboxOffsetY = 0;
        config.data[0].VFXOffsetX = 30;
        config.data[0].VFXOffsetY = -10;
        config.data[0].width = 90;
        config.data[0].height = 54;
        config.data[0].duration = 1;    // to be set by frame data IN SEC
        config.data[0].recovery = 0.3f;

        // attk 1
        config.data[1].damage = 20;
        config.data[1].hitboxOffsetX = 40;
        config.data[1].hitboxOffsetY = 0;
        config.data[1].VFXOffsetX = 50;
        config.data[1].VFXOffsetY = 0;
        config.data[1].width = 70;
        config.data[1].height = 54;
        config.data[1].duration = 1;    // to be set by frame data IN SEC
        config.data[1].recovery = 0.2f;

        // attk 2
        config.data[2].hitboxOffsetX = 60;
        config.data[2].hitboxOffsetY = 0;
        config.data[2].VFXOffsetX = 50;
        config.data[2].VFXOffsetY = 0;
        config.data[2].width = 160;
        config.data[2].height = 54;
        config.data[2].duration = 1;    // to be set by frame data IN SEC
        config.data[2].damage = 40;
        config.data[2].recovery = 0.2f;

        // rewind attk
        config.data[3].hitboxOffsetX = 60;
        config.data[3].hitboxOffsetY = 0;
        config.data[3].VFXOffsetX = 50;
        config.data[3].VFXOffsetY = 0;
        config.data[3].width = 160;
        config.data[3].height = 54;
        config.data[3].duration = 1;    // to be set by frame data IN SEC
        config.data[3].damage = 80;
        config.data[3].recovery = 0.2f;

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

        config.invicCooldown = 0.75;

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

        config.attackCooldown = 4;
        config.damage = 40;

        config.maxHp = 50;
        config.isFacingRight = true;

        config.moveSpeed = 200.0f;

        config.explosionRadius = 128;
        config.timeToExplode = 1.5;

        config.invicCooldown = 0.75;

        return config;
    }

    inline UGVConfig UGV()
    {
        UGVConfig config{};
        config.width = 64;
        config.height = 64;
        config.color = GREEN;
        config.gravity = 800.0f;
        config.maxFallSpeed = 1000.0f;
        config.checkGroundOffset = 30;

        config.maxHp = 8000;
        config.isFacingRight = true;

        config.attackWindupDuration = 2.0f;
        config.knockBackForce = 1000.0f;
        config.attackCooldown = 4;
        config.damage = 20;

        config.detectionDistance = 200.0f;
        config.attackDistance = 100;

        config.moveSpeed = 200.0f;

        config.invicCooldown = 0.5;

        return config;
    }
}

#endif //HEW_ENTITYCONFIGS_H
