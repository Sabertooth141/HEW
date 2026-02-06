//
// Created by saber on 1/14/2026.
//

#ifndef HEW_TILE_H
#define HEW_TILE_H
#include "../Lib/conioex.h"

enum class TileFlag : uint8_t
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
    {false, false, false, DARKGRAY},
    // SOLID
    {true, false, false, GREEN},
    // PLATFORM
    {false, true, false, BLUE},
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
    int tileID;

    Tile() : flag(TileFlag::AIR), tileID(-1)
    {
    }

    explicit Tile(const TileFlag flag) : flag(flag), tileID(-1)
    {
    }

    explicit Tile(const TileFlag flag, const int tileID) : flag(flag), tileID(tileID)
    {
    }

    [[nodiscard]] bool IsSolid() const { return GetTileProperties(flag).isSolid; }
    [[nodiscard]] bool IsPlatform() const { return GetTileProperties(flag).isPlatform; }
    [[nodiscard]] bool IsDamaging() const { return GetTileProperties(flag).isDamaging; }
    [[nodiscard]] COLORS GetColor() const { return GetTileProperties(flag).color; }
    [[nodiscard]] int GetTileID() const { return tileID; }
};

#endif //HEW_TILE_H
