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

enum TileFlags
{

};


class Tileset
{
    // functions
public:
    // loads sprite
    bool Load(const char* dir);
    [[nodiscard]] const TileDef& GetTileDef(uint16_t id) const;

    [[nodiscard]] bool IsSolid(uint16_t id) const;

    // vars
private:
    std::vector<TileDef> tiles;
};


#endif //HEW_TILESET_H
