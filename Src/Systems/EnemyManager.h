//
// Created by saber on 1/23/2026.
//

#ifndef HEW_ENEMYMANAGER_H
#define HEW_ENEMYMANAGER_H
#include <memory>
#include <vector>
#include "../Object/Entity/Enemy/Enemy.h"

struct EnemyManager
{
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    std::vector<std::unique_ptr<Enemy>> activeEnemies;

    void CreateEnemy(const EnemyConfig& config)
    {
        auto enemy = std::make_unique<Enemy>();
        enemy->Initialize(config);

        activeEnemies.push_back(std::move(enemy));
    }

    void UnregisterEnemy(Enemy* inEnemy)
    {
        std::erase_if(activeEnemies,
            [inEnemy](const std::unique_ptr<Enemy>& e) {
                return e.get() == inEnemy;
            });
    }

    [[nodiscard]] std::vector<Enemy*> GetActiveEnemies() const
    {
        std::vector<Enemy*> res;
        res.reserve(activeEnemies.size());
        for (const auto& e : activeEnemies)
        {
            res.push_back(e.get());
        }
        return res;
    }
};

#endif //HEW_ENEMYMANAGER_H