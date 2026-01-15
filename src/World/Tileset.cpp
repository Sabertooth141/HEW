//
// Created by saber on 1/14/2026.
//

#include "Tileset.h"

bool Tileset::Load(const char* dir, const TileFlags flag)
{
    if (dir == nullptr)
    {
        return false;
    }
    Bmp* sprite = LoadBmp(dir);

    if (sprite == nullptr)
    {
        return false;
    }

    TileDef tile = {};
    tile.flags = flag;
    tile.sprite = sprite;

    tiles.push_back(tile);
    return true;
}

TileDef Tileset::GetTileDef(uint16_t id) const
{
    return tiles[id];
}

bool Tileset::IsSolid(const uint16_t id) const
{
    return tiles[id].flags & TF_SOLID;
}


