//
// Created by saber on 1/16/2026.
//

#include "Tilemap.h"

#include "Camera.h"

Tilemap::Tilemap() : tiles(nullptr), widthTiles(0), heightTiles(0), tileSize(16)
{
}

Tilemap::~Tilemap()
{
    Shutdown();
}

bool Tilemap::Initialize(const int width, const int height, const int inTileSize)
{
    if (width <= 0 || height <= 0 || inTileSize <= 0)
    {
        return false;
    }

    this->widthTiles = width;
    this->heightTiles = height;
    this->tileSize = inTileSize;

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

void Tilemap::SetTile(const int x, const int y, const TileFlag flag, const int tileID) const
{
    if (!IsValidTile(x, y))
    {
        return;
    }

    if (tiles == nullptr)
    {
        return;
    }

    tiles[GetIndex(x, y)] = Tile(flag, tileID);
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

bool Tilemap::IsSolidAt(const float x, const float y) const
{
    const int tileX = WorldToTileX(x);
    const int tileY = WorldToTileY(y);

    if (!IsValidTile(tileX, tileY))
    {
        return false;
    }

    if (tiles == nullptr)
    {
        return false;
    }

    return tiles[GetIndex(tileX, tileY)].IsSolid();
}

bool Tilemap::IsPlatformAt(const float x, const float y) const
{
    const int tileX = WorldToTileX(x);
    const int tileY = WorldToTileY(y);

    if (!IsValidTile(tileX, tileY))
    {
        return false;
    }

    if (tiles == nullptr)
    {
        return false;
    }

    return tiles[GetIndex(tileX, tileY)].IsPlatform();
}

void Tilemap::Draw(const Camera& cam) const
{
    if (tiles == nullptr)
    {
        return;
    }

    float camLeft = cam.GetLeft();
    float camTop = cam.GetTop();
    float camRight = cam.GetRight();
    float camBottom = cam.GetBottom();

    int startDrawX = WorldToTileX(camLeft) - 1;
    int startDrawY = WorldToTileY(camTop) - 1;
    int endDrawX = WorldToTileX(camRight) + 1;
    int endDrawY = WorldToTileY(camBottom) + 1;

    if (startDrawX < 0)
    {
        startDrawX = 0;
    }
    if (startDrawY < 0)
    {
        startDrawY = 0;
    }

    if (endDrawX >= widthTiles)
    {
        endDrawX = widthTiles - 1;
    }
    if (endDrawY >= heightTiles)
    {
        endDrawY = heightTiles - 1;
    }

    for (int tileY = startDrawY; tileY <= endDrawY; tileY++)
    {
        for (int tileX = startDrawX; tileX <= endDrawX; tileX++)
        {
            Tile tile = GetTile(tileX, tileY);

            float worldX = TileToWorldX(tileX);
            float worldY = TileToWorldY(tileY);

            int screenX = cam.WorldToScreenX(worldX);
            int screenY = cam.WorldToScreenY(worldY);

            COLORS color = tile.GetColor();

            if (tile.flag == TileFlag::AIR)
            {
                DrawRect(screenX, screenY, screenX + tileSize - 1, screenY + tileSize - 1, color, true);
            }
            else
            {
                DrawRect(screenX, screenY, screenX + tileSize - 1, screenY + tileSize - 1, color);
            }

        }
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
