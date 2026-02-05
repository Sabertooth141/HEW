//
// Created by saber on 1/23/2026.
//

#ifndef HEW_ENEMYMANAGER_H
#define HEW_ENEMYMANAGER_H
#include <memory>
#include <vector>
#include "../Object/Entity/Enemy/Enemy.h"
#include "../Object/Entity/Enemy/Mine/Mine.h"

struct EnemyManager
{
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    std::vector<std::unique_ptr<Enemy>> activeEnemies;

    template<typename T, typename ConfigT>
    T* CreateEnemy(const ConfigT& config)
    {
        auto enemy = std::make_unique<T>();
        enemy->Initialize(config);

        activeEnemies.push_back(std::move(enemy));
        return static_cast<T*>(activeEnemies.back().get());
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