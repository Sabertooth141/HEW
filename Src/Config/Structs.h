//
// Created by saber on 1/23/2026.
//

#ifndef HEW_STRUCTS_H
#define HEW_STRUCTS_H

#include <memory>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#include "../Lib/conioex.h"
#include "../Animation/Animator.h"
#include <functional>

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

    /**
     * checks distance between point and self center point
     * @param position
     * @param distance
     * @return
     */
    bool CheckDistance(const Vector2 position, const float distance) const
    {
        float distanceToCompare = (position.x - topLeft.x) * (position.x - topLeft.x) + (position.y - topLeft.y) * (position.y - topLeft.y);
        return distanceToCompare < distance * distance;
    }

    static float GetDistance(const Vector2 position1, const Vector2 position2)
    {
        const float dx = position1.x - position2.x;
        const float dy = position1.y - position2.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    static Vector2 ToTopLeft(const Vector2 position, const Vector2 size)
    {
        return Vector2(position.x - size.x / 2, position.y - size.y / 2);
    }

    static Vector2 ToCenter(const Vector2 position, const Vector2 size)
    {
        return Vector2(position.x + size.x / 2, position.y + size.y / 2);
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
    float checkGroundOffset;
    float currHp;
    float maxHp;
    bool isFacingRight;

    float knockBackRecoveryTime;
};

struct PlayerConfig : EntityConfig
{
    float walkSpeed;
    float dashSpeed;
    float jumpForce;
    float timeStopDuration;
    float dashDuration;
    float airResistance;

    float trailFadeDuration;
};

class Entity;

struct EnemyConfig : EntityConfig
{
    Entity* target;

    float moveSpeed;
    float attackCooldown;

    float attackDistance;
    float detectionDistance;
    float damage;

    float invicCooldown;

    int spawnGroupID;
};

struct MineConfig : EnemyConfig
{
    float knockBackForce;

    float timeToExplode;
    float explosionRadius;
};

struct UGVConfig : EnemyConfig
{
    float knockBackForce;

    float attackWindupDuration;
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
    KeyBinding timeRewind = {PK_SHIFT};
};

// PLAYER STRUCTS

struct PlayerSnapshot
{
    Transform transform;
    bool isFacingRight;
    Bmp* frame = nullptr;
};

struct AttkData
{
    float damage;
    float hitboxOffsetX;
    float hitboxOffsetY;
    float VFXOffsetX;
    float VFXOffsetY;
    float width;
    float height;
    float duration;
    float recovery;
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

// ANIMATIONS

struct PlayerCombatAnimPaths
{
    PlayerCombatState animationState;
    std::string jsonPath;
    std::string bmpPath;
    int startFrame = 0;
};

template <typename T>
concept IsAllowedEnemyVFXType = std::is_same_v<T, EnemyVFXType> || std::is_same_v<T, EnemyState>;
template <IsAllowedEnemyVFXType T>
struct EnemyAnimPaths
{
    T animationName;
    std::string jsonPath;
    std::string bmpPath;
    int startFrame = 0;
};

// VFX
struct SplashTrailEffect
{
    Animator* animator;
    Transform transform;
    const Transform* followTargetTransform = nullptr;
    Vector2 offSet;
    bool isLoop;
    bool flipHorizontal;
    bool attached = false;
};

// MENU
struct TitleText
{
    Transform transform;
    const wchar_t* text;
    int fontSize;
    std::function<void()> onSelected;
};

#endif //HEW_STRUCTS_H
