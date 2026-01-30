//
// Created by saber on 1/23/2026.
//

#ifndef HEW_STRUCTS_H
#define HEW_STRUCTS_H

#include <memory>
#include <ranges>
#include <unordered_map>
#include <vector>

#include "../Lib/conioex.h"
#include "../Animation/Animator.h"

// MISC
struct Vector2
{
    float x, y;
};

struct Transform
{
    Vector2 topLeft, center, size;

    void CalculateCenterPosition()
    {
        const float centerX = topLeft.x + size.x / 2;
        const float centerY = topLeft.y + size.y / 2;
        center.x = centerX;
        center.y = centerY;
    }

    void CalculateTopLeftPosition()
    {
        const float topLeftX = center.x - size.x / 2;
        const float topLeftY = center.y - size.y / 2;
        topLeft.x = topLeftX;
        topLeft.y = topLeftY;
    }
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
};

struct PlayerConfig : EntityConfig
{
    float walkSpeed;
    float sprintSpeed;
    float jumpForce;
    float timeStopDuration;
    float airResistance;
};

struct EnemyConfig : EntityConfig
{
    Vector2 target;

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
    // common
    KeyBinding moveLeft = {PK_LEFT, PK_A};
    KeyBinding moveRight = {PK_RIGHT, PK_D};
    KeyBinding jump = {PK_SP, PK_W};
    KeyBinding attack = {PK_J};

    // time skills
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

struct AttkData
{
    float damage;
    float offsetX;
    float offsetY;
    float width;
    float height;
    float duration;
    float recovery;
    float comboWindow;
};

struct PlayerAttackConfig
{
    std::vector<AttkData> data;

    explicit PlayerAttackConfig(const int attackCnt)
    {
        data.reserve(attackCnt);
        for (int i = 0; i < attackCnt; i++)
        {
            data.emplace_back();
        }
    }
};

// ENEMY STRUCTS

#endif //HEW_STRUCTS_H
