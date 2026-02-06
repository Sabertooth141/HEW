//
// Created by saber on 1/24/2026.
//

#ifndef HEW_TILESET_H
#define HEW_TILESET_H
#include <vector>

#include "../Lib/conioex.h"

struct Tileset
{
    const Bmp* source{};
    int tileWidth{};
    int tileHeight{};
    std::vector<Bmp*> tiles{};
    std::vector<TileFlag> flags{};

    void LoadTileset(const Bmp* inSource, const int inTileWidth, const int inTileHeight)
    {
        source = inSource;
        tileWidth = inTileWidth;
        tileHeight = inTileHeight;

        for (auto& tile : tiles)
        {
            DeleteBmp(&tile);
        }
        tiles.clear();

        if (source == nullptr)
        {
            return;
        }

        if (source->pixel == nullptr)
        {
            return;
        }

        int cols = source->width / tileWidth;
        int rows = source->height / tileHeight;

        int bytesPerPixel = source->colbit <= 8 ? 1 : source->colbit / 8;

        for (int tileY = 0; tileY < rows; tileY++)
        {
            for (int tileX = 0; tileX < cols; tileX++)
            {
                Bmp* tile = static_cast<Bmp*>(calloc(1, sizeof(Bmp)));
                if (tile == nullptr)
                {
                    continue;
                }

                tile->width = tileWidth;
                tile->height = tileHeight;
                tile->colbit = source->colbit;
                tile->numpal = source->numpal;
                tile->numpal = source->numpal;
                tile->numpix = tileWidth * tileHeight * bytesPerPixel;
                tile->swapRB = source->swapRB;
                tile->aaLv = 0;
                tile->wch = 0;

                if (source->pal != nullptr && source->numpal > 0)
                {
                    tile->pal = static_cast<COLORREF*>(calloc(source->numpal, sizeof(COLORREF)));
                    if (tile->pal == nullptr)
                    {
                        DeleteBmp(&tile);
                        continue;
                    }

                    memcpy(tile->pal, source->pal, source->numpal * sizeof(COLORREF));
                }

                tile->pixel = static_cast<char*>(calloc(tile->numpix, sizeof(char)));
                if (tile->pixel == nullptr)
                {
                    DeleteBmp(&tile);
                    continue;
                }

                int startX = tileX * tileWidth;
                int startY = tileY * tileHeight;
                int srcStride = source->width * bytesPerPixel;

                for (int y = 0; y < tileHeight; y++)
                {
                    const int srcIndex = (startY + y) * srcStride + startX * bytesPerPixel;
                    const int destIndex = y * tileWidth * bytesPerPixel;
                    memcpy(&tile->pixel[destIndex], &source->pixel[srcIndex], tileWidth * bytesPerPixel);
                }

                tiles.push_back(tile);
            }
        }

        flags.resize(tiles.size(), TileFlag::AIR);

        for (int y = 0; y < source->height / tileHeight; y++)
        {
            for (int x = 0; x < source->width / tileWidth; x++)
            {
                const int index = y * (source->width / tileWidth) + x;
                if (x <= 2 && y <= 2)
                {
                    flags[index] = TileFlag::SOLID;
                }
                else if (x <= 2 && y == 3)
                {
                    flags[index] = TileFlag::PLATFORM;
                }
            }
        }
    }

    Bmp* GetTile(const int index) const
    {
        if (index < 0 || index >= tiles.size())
        {
            return nullptr;
        }

        return tiles[index];
    }

    ~Tileset()
    {
        for (auto& tile : tiles)
        {
            DeleteBmp(&tile);
        }
        tiles.clear();
        source = nullptr;
    }
};

#endif //HEW_TILESET_H
