//
// Created by saber on 1/23/2026.
//

#ifndef HEW_STRUCTS_H
#define HEW_STRUCTS_H

#include "../Lib/conioex.h"

// MISC

struct TargetPosition
{
    float x;
    float y;
};

// SYSTEM STRUCTS

struct TimeManagerConfig
{
    // TIME STOP
    float timeStopCooldownMax;

    // TIME REWIND
    float rewindCooldownMax;
    float rewindMagnitude;
};

struct VFXConfig
{
    COLORREF normalPal[16];
    COLORREF grayscale[16];
};

// ENTITY CONFIGS

struct ObjectConfig
{
    float x, y, width, height;
    COLORS color;
};

struct EntityConfig : ObjectConfig
{
    float velX;
    float velY;
    float gravity;
    float maxFallSpeed;
    float currHp;
    float maxHp;
    bool isFacingRight;

    float damage;
    float attackCooldown;
    float attackOffsetX;
    float attackOffsetY;
    float attackWidth;
    float attackHeight;

    float attackDuration;
};

struct PlayerConfig : EntityConfig
{
    float walkSpeed;
    float sprintSpeed;
    float jumpForce;
    float timeStopDuration;
};

struct EnemyConfig : EntityConfig
{
    TargetPosition target;

    float moveSpeed;
    float attackCooldown;
};

// INPUT CONFIGS

struct KeyBinding
{
    int primary = -1;
    int secondary = -1;

    [[nodiscard]] bool IsPressed() const
    {
        return ChkKeyPress(primary) || ChkKeyPress(secondary);
    }

    [[nodiscard]] bool IsEdge() const
    {
        return ChkKeyEdge(primary) || ChkKeyEdge(secondary);
    }
};

struct GlobalInputConfig
{
    KeyBinding quitGame = {PK_ESC};
};

struct InputConfig
{
    KeyBinding moveLeft = {PK_LEFT, PK_A};
    KeyBinding moveRight = {PK_RIGHT, PK_D};
    KeyBinding jump = {PK_SP, PK_W};
    KeyBinding attack = {PK_J};

    KeyBinding timeStop = {PK_K};
    KeyBinding timeRewind = {PK_L};
};

// PLAYER STRUCTS

struct PlayerSnapshot
{
    float x, y;
    float velX, velY;
    bool isFacingRight;
};

// ENEMY STRUCTS


#endif //HEW_STRUCTS_H
