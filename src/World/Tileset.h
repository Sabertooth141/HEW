//
// Created by saber on 1/14/2026.
//

#ifndef HEW_TILESET_H
#define HEW_TILESET_H
#include <cstdint>
#include <vector>

#include "../conioex.h"

struct TileDef
{
    Bmp* sprite;
    uint16_t flags;
};

struct TileMap
{
    int width;
    int height;

    std::vector<uint16_t> tiles;
};

enum TileFlags : uint16_t
{
    TF_NONE = 0,
    TF_SOLID = 1 << 0,
    TF_WALL = 2 << 0
};

class Tileset
{
    // functions
public:
    // loads sprite
    bool Load(const char* dir, TileFlags flag);

    TileDef GetTileDef(uint16_t id) const;

    bool IsSolid(uint16_t id) const;

    // vars
private:
    std::vector<TileDef> tiles;
};


#endif //HEW_TILESET_H
