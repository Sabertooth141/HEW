//
// Created by saber on 1/23/2026.
//

#ifndef HEW_ENEMYMANAGER_H
#define HEW_ENEMYMANAGER_H
#include <memory>
#include <vector>
#include "../Object/Entity/Enemy/Enemy.h"

class UGV;
class Mine;

// Helper template to make static_assert depend on template parameter
template<typename>
constexpr bool always_false_v = false;

struct EnemyManager
{
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    std::vector<std::unique_ptr<Enemy>> activeEnemies;

    template <typename T, typename ConfigT>
    T* CreateEnemy(const ConfigT& config)
    {
        auto enemy = std::make_unique<T>();
        enemy->Initialize(config);

        const auto& paths = [this]() -> const auto&
        {
            if constexpr (std::is_same_v<T, Mine>)
            {
                return mineAnimationPaths;
            }
            else if constexpr (std::is_same_v<T, UGV>)
            {
                return UGVAnimationPaths;
            }
            else
            {
                static_assert(always_false_v<T>, "No animation path defined for enemy type");
            }
        }();

        for (const auto& animation : paths)
        {
            enemy->InitAnimation(animation);
        }

        activeEnemies.push_back(std::move(enemy));

        return static_cast<T*>(activeEnemies.back().get());
    }

    void UnregisterEnemy(Enemy* inEnemy)
    {
        std::erase_if(activeEnemies,
                      [inEnemy](const std::unique_ptr<Enemy>& e)
                      {
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

private:
    // ANIMATIONS
    // mine

    std::vector<EnemyAnimPaths<EnemyState>> mineAnimationPaths =
    {
        {
            EnemyState::PATROL,
            "../Assets/Enemy/Mine/MineIdle/MineIdle.json",
            "../Assets/Enemy/Mine/MineIdle/MineIdle.bmp"
        },
        {
            EnemyState::ATTK,
            "../Assets/Enemy/Mine/MineAttk/MineAttk.json",
            "../Assets/Enemy/Mine/MineAttk/MineAttk.bmp"
        }
    };

    // UGV
    std::vector<EnemyAnimPaths<EnemyState>> UGVAnimationPaths =
    {
        {
            EnemyState::PATROL,
            "../Assets/Enemy/UGV/UGVIdle/UGVIdle.json",
            "../Assets/Enemy/UGV/UGVIdle/UGVIdle.bmp"
        },
        {
            EnemyState::PATHFIND,
            "../Assets/Enemy/UGV/UGVIdle/UGVIdle.json",
            "../Assets/Enemy/UGV/UGVIdle/UGVIdle.bmp"
        }
    };
};

#endif //HEW_ENEMYMANAGER_H