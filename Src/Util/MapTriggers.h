//
// Created by saber on 2/14/2026.
//

#ifndef HEW_MAPTRIGGERS_H
#define HEW_MAPTRIGGERS_H
#include <vector>
#include "../Config/Structs.h"

enum class EnemyType : uint8_t;

namespace TriggerTileID
{
    constexpr int MINE_SPAWNER = 16;
    constexpr int UGV_SPAWNER = 25;
    constexpr int UAV_SPAWNER = 34;
    constexpr int BARRIER = 17;
    constexpr int VICTORY = 8;
}

struct SpawnTrigger
{
    int id;
    Transform transform;

    bool activated;

    // enemy spawn settings
    int enemyCnt;
    float spawnOffsetX;
    float spawnOffsetY;
    EnemyType type;
    int enemySpawned;

    SpawnTrigger() : id(-1), transform(), activated(false),
                     enemyCnt(2), spawnOffsetX(0), spawnOffsetY(-64), type(),
                     enemySpawned(0)
    {
    }
};

struct Barrier
{
    int groupID;
    Transform transform;
    bool active;

    int tileX, tileY;

    std::vector<int> linkedSpawnerIDs;

    Barrier() : groupID(-1), transform(),
                active(false), tileX(-1), tileY(-1), linkedSpawnerIDs(0)
    {
    }
};

struct VictoryTrigger
{
    Transform transform;
    bool triggered;

    VictoryTrigger() : transform(), triggered(false)
    {
    }
};

#endif //HEW_MAPTRIGGERS_H
