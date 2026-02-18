//
// Created by saber on 2/16/2026.
//

#include "TriggerManager.h"

#include "../Config/EntityConfigs.h"
#include "../Systems/EnemyManager.h"
#include "../World/Camera.h"
#include "../Object/Entity/Enemy/Mine/Mine.h"
#include "../Object/Entity/Enemy/UGV/UGV.h"
#include "../Systems/GameManager.h"

void TriggerManager::Update(PlayerController& player, float deltaTime)
{
    HandleSpawnTriggers(player);
    HandleBarriers();
    HandleVictoryTrigger(player);
}

void TriggerManager::ScanAndBuildTriggers(std::vector<uint8_t>& mapData, const int mapWidth, const int mapHeight,
                                          const int inTileSize)
{
    Clear();

    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            int index = y * mapWidth + x;
            uint8_t tileVal = mapData[index];

            float worldX = static_cast<float>(x * inTileSize);
            float worldY = static_cast<float>(y * inTileSize);
            float tileSize = static_cast<float>(inTileSize);

            switch (tileVal)
            {
            case TriggerTileID::MINE_SPAWNER:
                {
                    SpawnTrigger trigger;
                    trigger.id = nextSpawnId;
                    nextSpawnId += 1;
                    trigger.transform.center.x = worldX;
                    trigger.transform.center.y = worldY;
                    trigger.transform.size.x = tileSize * 2;
                    trigger.transform.size.y = tileSize * 40;
                    trigger.transform.CalculateTopLeftPosition();
                    trigger.activated = false;
                    trigger.type = EnemyType::MINE;
                    trigger.enemyCnt = 2;
                    trigger.spawnOffsetX = 0;
                    trigger.spawnOffsetY = -32;

                    spawnTriggers.push_back(trigger);
                    mapData[index] = 255;
                    break;
                }
            case TriggerTileID::UGV_SPAWNER:
                {
                    SpawnTrigger trigger;
                    trigger.id = nextSpawnId;
                    nextSpawnId += 1;
                    trigger.transform.center.x = worldX;
                    trigger.transform.center.y = worldY;
                    trigger.transform.size.x = tileSize * 2;
                    trigger.transform.size.y = tileSize * 40;
                    trigger.transform.CalculateTopLeftPosition();
                    trigger.activated = false;
                    trigger.type = EnemyType::UGV;
                    trigger.enemyCnt = 1;
                    trigger.spawnOffsetX = 0;
                    trigger.spawnOffsetY = -32;

                    spawnTriggers.push_back(trigger);
                    mapData[index] = 255;
                    break;
                }
            case TriggerTileID::BARRIER:
                {
                    Barrier barrier;
                    barrier.transform.topLeft.x = worldX;
                    barrier.transform.topLeft.y = worldY;
                    barrier.transform.size.x = tileSize;
                    barrier.transform.size.y = tileSize;
                    barrier.transform.CalculateCenterPosition();
                    barrier.active = true;
                    barrier.tileX = x;
                    barrier.tileY = y;

                    barriers.push_back(barrier);

                    mapData[index] = 1;
                    break;
                }
            case TriggerTileID::VICTORY:
                {
                    VictoryTrigger victoryTrigger;
                    victoryTrigger.transform.topLeft.x = worldX;
                    victoryTrigger.transform.topLeft.y = worldY;
                    victoryTrigger.transform.size.x = tileSize;
                    victoryTrigger.transform.size.y = tileSize;
                    victoryTrigger.transform.CalculateCenterPosition();
                    victoryTrigger.triggered = false;

                    victoryTriggers.push_back(victoryTrigger);
                    mapData[index] = 255;
                    break;
                }
            default:
                {
                    break;
                }
            }
        }
    }

    LinkBarriersToSpawns();
}

void TriggerManager::LinkBarriersToSpawns()
{
    for (auto& barrier : barriers)
    {
        for (const auto& trigger : spawnTriggers)
        {
            float dx = barrier.transform.center.x - trigger.transform.center.x;
            float dy = barrier.transform.center.y - trigger.transform.center.y;
            if (dx < 0)
            {
                continue;
            }

            float distance = sqrt(dx * dx + dy * dy);

            if (distance < SPAWN_LINK_RANGE)
            {
                bool alreadyLinked = false;

                for (const int id : barrier.linkedSpawnerIDs)
                {
                    if (id == trigger.id)
                    {
                        alreadyLinked = true;
                        break;
                    }
                }

                if (!alreadyLinked)
                {
                    barrier.linkedSpawnerIDs.push_back(trigger.id);
                }
            }
        }
    }
}

bool TriggerManager::Overlaps(const Transform& a, const Transform& b)
{
    return a.topLeft.x < b.topLeft.x && a.topLeft.x + a.size.x > b.topLeft.x &&
        a.topLeft.y < b.topLeft.y + b.size.y && a.topLeft.y + a.size.y > b.topLeft.y;
}

bool TriggerManager::PlayerOverlaps(const PlayerController& player, const Transform& rectTransform)
{
    const Transform& playerTransform = player.transform;
    return Overlaps(playerTransform, rectTransform);
}

void TriggerManager::HandleSpawnTriggers(PlayerController& player)
{
    for (auto& trigger : spawnTriggers)
    {
        if (trigger.activated)
        {
            continue;
        }

        if (PlayerOverlaps(player, trigger.transform))
        {
            trigger.activated = true;

            for (int i = 0; i < trigger.enemyCnt; i++)
            {
                switch (trigger.type)
                {
                case EnemyType::MINE:
                    {
                        SpawnEnemyGroup<Mine, MineConfig>(trigger, i, player, config::Mine);
                        break;
                    }
                case EnemyType::UGV:
                    {
                        SpawnEnemyGroup<UGV, UGVConfig>(trigger, i, player, config::UGV);
                        break;
                    }
                default:
                    break;
                }
            }
        }

        trigger.enemySpawned = trigger.enemyCnt;
    }
}

void TriggerManager::HandleBarriers()
{
    for (auto& barrier : barriers)
    {
        if (!barrier.active)
        {
            continue;
        }

        bool allCleared = true;
        bool spawnerActivated = false;

        for (const int id : barrier.linkedSpawnerIDs)
        {
            if (!spawnTriggers[id].activated)
            {
                allCleared = false;
                break;
            }

            spawnerActivated = true;

            if (EnemyManager::Instance().CountAliveInGroup(id) > 0)
            {
                allCleared = false;

                break;
            }
        }

        if (spawnerActivated && allCleared)
        {
            barrier.active = false;

            if (tilemap != nullptr)
            {
                tilemap->SetTile(barrier.tileX, barrier.tileY, TileFlag::AIR);
            }
        }
    }
}

void TriggerManager::HandleVictoryTrigger(const PlayerController& player)
{
    for (auto& trigger : victoryTriggers)
    {
        if (trigger.triggered)
        {
            continue;
        }

        if (PlayerOverlaps(player, trigger.transform))
        {
            trigger.triggered = true;
            GameManager::Instance().SetIsVictory(true);
        }
    }
}

void TriggerManager::DebugDraw(const Camera& camera) const
{
    for (const auto& trigger : spawnTriggers)
    {
        int screenX = camera.WorldToScreenX(trigger.transform.topLeft.x);
        int screenY = camera.WorldToScreenY(trigger.transform.topLeft.y);

        COLORS color = trigger.activated ? BLUE : GREEN;
        DrawRect(screenX, screenY, screenX + static_cast<int>(trigger.transform.size.x),
                 screenY + static_cast<int>(trigger.transform.size.y), color, false);
    }

    for (const auto& trigger : victoryTriggers)
    {
        int screenX = camera.WorldToScreenX(trigger.transform.topLeft.x);
        int screenY = camera.WorldToScreenY(trigger.transform.topLeft.y);

        COLORS color = trigger.triggered ? DARKGRAY : RED;
        DrawRect(screenX, screenY, screenX + static_cast<int>(trigger.transform.size.x),
                 screenY + static_cast<int>(trigger.transform.size.y), color, false);
    }
}

template <typename EnemyT, typename ConfigT>
void TriggerManager::SpawnEnemyGroup(SpawnTrigger& trigger, int i, PlayerController& player, ConfigT (*configFunc)())
{
    ConfigT cfg = configFunc();

    float spread = static_cast<float>(i - trigger.enemyCnt / 2) * 80;
    cfg.x = trigger.transform.center.x + spread + trigger.spawnOffsetX;
    cfg.y = trigger.transform.center.y + trigger.spawnOffsetY;
    cfg.spawnGroupID = trigger.id;
    cfg.target = &player;

    EnemyManager::Instance().CreateEnemy<EnemyT, ConfigT>(cfg);
}


void TriggerManager::Clear()
{
    spawnTriggers.clear();
    barriers.clear();
    victoryTriggers.clear();
    nextSpawnId = 0;
}
