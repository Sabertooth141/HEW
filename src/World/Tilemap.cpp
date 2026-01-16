//
// Created by saber on 1/16/2026.
//

#include "Tilemap.h"

Tilemap::Tilemap() : tiles(nullptr), widthTiles(0), heightTiles(0), tileSize(16)
{
}

Tilemap::~Tilemap()
{
    Shutdown();
}

bool Tilemap::Initialize(const int width, const int height, const int tileSize)
{
    if (widthTiles <= 0 || heightTiles <= 0 || tileSize <= 0)
    {
        return false;
    }

    this->widthTiles = width;
    this->heightTiles = height;
    this->tileSize = tileSize;

    const int totalTiles = this->widthTiles * this->heightTiles;
    this->tiles = new Tile[totalTiles];

    for (int i = 0; i < totalTiles; i++)
    {
        this->tiles[i] = Tile(TileFlag::AIR);
    }

    return true;
}

void Tilemap::Shutdown()
{
    if (tiles != nullptr)
    {
        delete[] tiles;
        tiles = nullptr;
    }

    widthTiles = 0;
    heightTiles = 0;
}

Tile Tilemap::GetTile(const int x, const int y) const
{
    if (!IsValidTile(x, y))
    {
        return Tile(TileFlag::AIR);
    }

    if (tiles == nullptr)
    {
        return Tile(TileFlag::AIR);
    }

    return tiles[GetIndex(x, y)];
}

void Tilemap::SetTile(const int x, const int y, const TileFlag flag) const
{
    if (!IsValidTile(x, y))
    {
        return;
    }

    if (tiles == nullptr)
    {
        return;
    }

    tiles[GetIndex(x, y)] = Tile(flag);
}

Tile Tilemap::GetTileInWorld(const float x, const float y) const
{
    return GetTile(WorldToTileX(x), WorldToTileY(y));
}

int Tilemap::WorldToTileX(const float worldX) const
{
    return static_cast<int>(worldX ) / tileSize;
}

int Tilemap::WorldToTileY(const float worldY) const
{
    return static_cast<int>(worldY) / tileSize;
}

float Tilemap::TileToWorldX(const int tileX) const
{
    return static_cast<float>(tileX * tileSize);
}

float Tilemap::TileToWorldY(const int tileY) const
{
    return static_cast<float>(tileY * tileSize);
}

bool Tilemap::IsSolidAt(const int x, const int y) const
{
    if (!IsValidTile(x, y))
    {
        return false;
    }

    if (tiles == nullptr)
    {
        return false;
    }

    return tiles[GetIndex(x, y)].IsSolid();
}

bool Tilemap::IsPlatformAt(const int x, const int y) const
{
    if (!IsValidTile(x, y))
    {
        return false;
    }

    if (tiles == nullptr)
    {
        return false;
    }

    return tiles[GetIndex(x, y)].IsPlatform();
}

void Tilemap::Draw(const Camera& cam)
{
    if (tiles == nullptr)
    {
        return;
    }


}

bool Tilemap::LoadFromArr(const unsigned char* data, const int width, const int height)
{
    if (!Initialize(width, height, tileSize))
    {
        return false;
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            unsigned char tileValue = data[x + y * width];
            auto flag = TileFlag::AIR;

            if (tileValue < static_cast<unsigned char>(TileFlag::COUNT))
            {
                flag = static_cast<TileFlag>(tileValue);
            }

            SetTile(x, y, flag);
        }
    }

    return true;
}

int Tilemap::GetIndex(const int tileX, const int tileY) const
{
    return tileY * widthTiles + tileX;
}

bool Tilemap::IsValidTile(const int tileX, const int tileY) const
{
    return tileX >= 0 && tileX < widthTiles && tileY >= 0 && tileY < heightTiles;
}
