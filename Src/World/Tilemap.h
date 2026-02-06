//
// Created by saber on 1/16/2026.
//

#ifndef HEW_TILEMAP_H
#define HEW_TILEMAP_H
#include <complex.h>

#include "Tile.h"
#include "Tileset.h"
#include "../World/Camera.h"

class Tilemap
{
public:
    Tilemap();
    ~Tilemap();

    // GAME LOOP
    bool Initialize(int width, int height, int inTileSize);
    void Shutdown();

    // TILE ACCESS
    [[nodiscard]] Tile GetTile(int x, int y) const;
    void SetTile(int x, int y, TileFlag flag = TileFlag::COUNT, int tileID = -1) const;

    // WORLD COORD CONVERSION
    [[nodiscard]] Tile GetTileInWorld(float x, float y) const;
    [[nodiscard]] int WorldToTileX(float worldX) const;
    [[nodiscard]] int WorldToTileY(float worldY) const;
    [[nodiscard]] float TileToWorldX(int tileX) const;
    [[nodiscard]] float TileToWorldY(int tileY) const;

    // COLLISION
    [[nodiscard]] bool IsSolidAt(float x, float y) const;
    [[nodiscard]] bool IsPlatformAt(float x, float y) const;

    // RENDERING
    void Draw(const Camera& cam) const;

    // MAP LOADING
    bool LoadFromArr(const std::vector<uint8_t>& data, int width, int height, Tileset& inTileset, int inTileSize);
    static std::vector<uint8_t> ParseMapCSV(const std::string& filePath, int& mapWidth, int& mapHeight);

    // GETTERS
    [[nodiscard]] int GetWidthTiles() const { return widthTiles; }
    [[nodiscard]] int GetHeightTiles() const { return heightTiles; }
    [[nodiscard]] int GetTileSize() const { return tileSize; }
    [[nodiscard]] int GetWidthPixels() const { return widthTiles * tileSize; }
    [[nodiscard]] int GetHeightPixels() const { return heightTiles * tileSize; }

private:
    Tile* tiles;
    Tileset* tileset;
    int widthTiles;
    int heightTiles;
    int tileSize;

    [[nodiscard]] int GetIndex(int tileX, int tileY) const;
    [[nodiscard]] bool IsValidTile(int tileX, int tileY) const;
};


#endif //HEW_TILEMAP_H
