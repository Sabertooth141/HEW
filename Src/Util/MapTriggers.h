//
// Created by saber on 2/14/2026.
//

#ifndef HEW_MAPTRIGGERS_H
#define HEW_MAPTRIGGERS_H
#include <vector>

namespace TriggerTileID
{
    constexpr int SPAWN_TRIGGER = 23;
    constexpr int BARRIER = 15;
    constexpr int VICTORY = 31;
}

struct SpawnTrigger
{
    int id;
    float x, y;
    float width, height;

    bool activated;

    // enemy spawn settings
    int enemyCnt;
    float spawnOffsetX;
    float spawnOffsetY;

    int enemySpawned;

    SpawnTrigger() : id(-1), x(0), y(0), width(0), height(0), activated(false),
                     enemyCnt(2), spawnOffsetX(0), spawnOffsetY(-64),
                     enemySpawned(0)
    {
    }
};

struct Barrier
{
    int groupID;
    float x, y;
    float width, height;
    bool active;

    int tileX, tileY;

    std::vector<int> linkedSpawnerIDs;

    Barrier() : groupID(-1), x(0), y(0), width(0), height(0),
                active(false), tileX(-1), tileY(-1), linkedSpawnerIDs(0)
    {
    }
};

struct VictoryTrigger
{
    float x, y;
    float width, height;
    bool triggered;

    VictoryTrigger() : x(0), y(0), width(0), height(0), triggered(false)
    {
    }
};

#endif //HEW_MAPTRIGGERS_H
