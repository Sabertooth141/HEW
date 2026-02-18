//
// Created by saber on 2/16/2026.
//

#ifndef HEW_TRIGGERMANAGER_H
#define HEW_TRIGGERMANAGER_H
#include <vector>
#include "MapTriggers.h"
#include "../Object/Entity/Player/PlayerController.h"

#define SPAWN_LINK_RANGE 2000


struct EnemyManager;
class PlayerController;
class Camera;
class Tilemap;

class TriggerManager
{
public:
    static TriggerManager& Instance()
    {
        static TriggerManager instance;
        return instance;
    }

    void Update(PlayerController& player, float deltaTime);

    void ScanAndBuildTriggers(std::vector<uint8_t>& mapData, int mapWidth, int mapHeight, int inTileSize);
    void LinkBarriersToSpawns();

    static bool Overlaps(const Transform& a, const Transform& b);
    static bool PlayerOverlaps(const PlayerController& player, const Transform& rectTransform);

    void HandleSpawnTriggers(PlayerController& player);
    void HandleBarriers();
    void HandleVictoryTrigger(const PlayerController& player);

    template<typename EnemyT, typename ConfigT>
    void SpawnEnemyGroup(SpawnTrigger& trigger, int i, PlayerController& player, ConfigT(*configFunc)());

    void SetTileMap(Tilemap* inTilemap) { tilemap = inTilemap; }

    void DebugDraw(const Camera& camera) const;

    void Clear();

private:
    std::vector<SpawnTrigger> spawnTriggers;
    std::vector<Barrier> barriers;
    std::vector<VictoryTrigger> victoryTriggers;

    Tilemap* tilemap = nullptr;
    int nextSpawnId = 0;
};


#endif //HEW_TRIGGERMANAGER_H
