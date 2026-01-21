//
// Created by saber on 1/14/2026.
//

#ifndef HEW_TILE_H
#define HEW_TILE_H
#include <cstdint>
#include "../Lib/conioex.h"

enum class TileFlag : unsigned char
{
    AIR = 0,
    SOLID,
    PLATFORM,
    SPIKE,
    COUNT
};

struct TileProperties
{
    bool isSolid;
    bool isPlatform;
    bool isDamaging;
    COLORS color;
};

inline constexpr TileProperties TILE_PROPERTIES[] =
{
    // AIR
    {false, false, false, BLACK},
    // SOLID
    {true, false, false, GREEN},
    // PLATFORM
    {false, true, false, DARKGRAY},
    // SPIKE
    {true, false, true, RED}
};

inline const TileProperties& GetTileProperties(TileFlag flag)
{
    return TILE_PROPERTIES[static_cast<int>(flag)];
}

struct Tile
{
    TileFlag flag;

    Tile() : flag(TileFlag::AIR)
    {
    }

    explicit Tile(const TileFlag flag) : flag(flag)
    {
    }

    [[nodiscard]] bool IsSolid() const { return GetTileProperties(flag).isSolid; }
    [[nodiscard]] bool IsPlatform() const { return GetTileProperties(flag).isPlatform; }
    [[nodiscard]] bool IsDamaging() const { return GetTileProperties(flag).isDamaging; }
    [[nodiscard]] COLORS GetColor() const { return GetTileProperties(flag).color; }
};

#endif //HEW_TILE_H
