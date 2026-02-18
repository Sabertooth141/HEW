//
// Created by saber on 1/14/2026.
//

#include "GameScene.h"

#include <format>

#include "../../Animation/SpriteSheetLoader.h"
#include "../../Config/EntityConfigs.h"
#include "../../Config/SystemConfigs.h"
#include "../../Lib/conioex_custom.h"
#include "../../Systems/EnemyManager.h"
#include "../../VFX/AttackVFXManager.h"
#include "../../Object/Entity/Enemy/UGV/UGV.h"
#include "../Game.h"
#include "../../Systems/GameManager.h"
#include "../../Util/TriggerManager.h"

#define TEST_MAP_WIDTH 100
#define TEST_MAP_HEIGHT 50

// ====================ANIM PATHS======================================
// player
std::vector<PlayerNormalAnimPaths> playerNormalAnimationPaths =
{
    {
        PlayerNormalState::MOVE,
        "../Assets/Player/PlayerMove/PlayerCharacterMove.json",
        "../Assets/Player/PlayerMove/PlayerCharacterMove.bmp", 1
    },
    {
        PlayerNormalState::IDLE,
        "../Assets/Player/PlayerIdle/PlayerCharacterIdle.json",
        "../Assets/Player/PlayerIdle/PlayerCharacterIdle.bmp"
    },
    {
        PlayerNormalState::KNOCKBACK,
        "../Assets/Player/PlayerKnockedBack/PlayerCharacterKnockedBack.json",
        "../Assets/Player/PlayerKnockedBack/PlayerCharacterKnockedBack.bmp"
    }
};

std::vector<PlayerCombatAnimPaths> playerCombatAnimationPaths =
{
    {
        PlayerCombatState::ATTK0,
        "../Assets/Player/PlayerAttk/PlayerCharacterAttk0.json",
        "../Assets/Player/PlayerAttk/PlayerCharacterAttk0.bmp"
    },
    {
        PlayerCombatState::ATTK1,
        "../Assets/Player/PlayerAttk/PlayerCharacterAttk1.json",
        "../Assets/Player/PlayerAttk/PlayerCharacterAttk1.bmp"
    },
    {
        PlayerCombatState::ATTK2,
        "../Assets/Player/PlayerAttk/PlayerCharacterAttk2.json",
        "../Assets/Player/PlayerAttk/PlayerCharacterAttk2.bmp"
    },
    {
        PlayerCombatState::REWIND_ATTK,
        "../Assets/Player/PlayerAttk/PlayerCharacterRewindAttk.json",
        "../Assets/Player/PlayerAttk/PlayerCharacterRewindAttk.bmp"
    }
};

// VFX
// player
std::vector<PlayerCombatAnimPaths> playerAttackVFXPaths =
{
    {
        PlayerCombatState::ATTK0,
        "../Assets/Player/PlayerAttk/AttkVFX/PlayerCharacterAttk0Effect.json",
        "../Assets/Player/PlayerAttk/AttkVFX/PlayerCharacterAttk0Effect.bmp"
    },
    {
        PlayerCombatState::ATTK1,
        "../Assets/Player/PlayerAttk/AttkVFX/PlayerCharacterAttk1Effect.json",
        "../Assets/Player/PlayerAttk/AttkVFX/PlayerCharacterAttk1Effect.bmp"
    },
    {
        PlayerCombatState::ATTK2,
        "../Assets/Player/PlayerAttk/AttkVFX/PlayerCharacterAttk2Effect.json",
        "../Assets/Player/PlayerAttk/AttkVFX/PlayerCharacterAttk2Effect.bmp"
    }

};

// enemy attack vfx
std::vector<EnemyAnimPaths<EnemyVFXType>> enemyAttackVFXPaths =
{
    {
        EnemyVFXType::MINE,
        "../Assets/Enemy/Mine/AttkVFX/MineAttkVFX.json",
        "../Assets/Enemy/Mine/AttkVFX/MineAttkVFX.bmp"
    }
};

// enemy hit vfx
std::vector<EnemyAnimPaths<EnemyVFXType>> enemyHitVFXPaths =
{
    {
        EnemyVFXType::HIT,
        "../Assets/Enemy/EnemyHitEffect/EnemyHitEffect.json",
        "../Assets/Enemy/EnemyHitEffect/EnemyHitEffect.bmp"
    }
};

// ========================================================================

GameScene::GameScene()
= default;

bool GameScene::Initialize()
{
    // init tileMap
    LoadTileset("../Assets/Tileset/SceneTileset/Tileset.bmp");

    int mapWidth, mapHeight;
    std::vector<uint8_t> mapData = tileMap.ParseMapCSV("../Assets/Maps/MapCsv/Scene1.csv", mapWidth, mapHeight);

    TriggerManager::Instance().ScanAndBuildTriggers(mapData, mapWidth, mapHeight, TILE_SIZE);

    tileMap.LoadFromArr(mapData, mapWidth, mapHeight, tileset, TILE_SIZE);
    TriggerManager::Instance().SetTileMap(&tileMap);

    // init vfxmanager
    for (const auto& effect : playerAttackVFXPaths)
    {
        AttackVFXManager::Instance().InitAnimation(effect);
    }

    for (const auto& effect : enemyAttackVFXPaths)
    {
        AttackVFXManager::Instance().InitAnimation(effect);
    }

    for (const auto& effect : enemyHitVFXPaths)
    {
        AttackVFXManager::Instance().InitAnimation(effect);
    }

    // TODO: TEMP
    const float playerStartX = 128;
    const float playerStartY = 440;

    // playerController
    PlayerConfig playerCfg = config::Player();
    PlayerAttackConfig playerAttackConfig = config::PlayerAttack();
    playerCfg.x = playerStartX;
    playerCfg.y = playerStartY;

    playerController.Initialize(playerCfg, playerAttackConfig);
    for (const auto& animation : playerNormalAnimationPaths)
    {
        playerController.InitAnimation(animation);
    }

    for (const auto& animation : playerCombatAnimationPaths)
    {
        playerController.InitAttackAnimation(animation);
    }

    GameManager::Instance().RegisterPlayer(playerController);

    // for (int x = 0; x < mapWidth; x++)
    // {
    //     for (int y = 0 ; y < mapHeight; y++)
    //     {
    //         if (tileMap.GetTile(x, y).GetTileID() == 23)
    //         {
    //             MineConfig mineCfg = config::Mine();
    //             mineCfg.x = tileMap.TileToWorldX(x);
    //             mineCfg.y = tileMap.TileToWorldY(y);
    //             mineCfg.target = &playerController;
    //
    //             EnemyManager::Instance().CreateEnemy<Mine, MineConfig>(mineCfg);
    //         }
    //     }
    // }
    //

    // MineConfig mineCfg = config::Mine();
    // mineCfg.x = playerStartX + 100;
    // mineCfg.y = playerStartY;
    // mineCfg.target = &playerController;
    // mineCfg.maxHp = 10000;

    // EnemyManager::Instance().CreateEnemy<Mine, MineConfig>(mineCfg);

    // UGVConfig ugvCfg = config::UGV();
    // ugvCfg.x = playerStartX;
    // ugvCfg.y = playerStartY;
    // ugvCfg.target = &playerController;
    //
    // EnemyManager::Instance().CreateEnemy<UGV, UGVConfig>(ugvCfg);

    // subsystems
    const VFXConfig vfxCfg = sysCfg::VFXCfg();
    vfxManager.Initialize(vfxCfg);

    const TimeManagerConfig timeManagerCfg = sysCfg::TimeCfg();
    TimeManager::Instance().Initialize(timeManagerCfg);

    // camera
    Camera::Instance().Initialize(static_cast<float>(GameConfig::VIEW_WIDTH) / 2,
                                  static_cast<float>(GameConfig::VIEW_HEIGHT) / 2,
                                  GameConfig::VIEW_WIDTH,
                                  GameConfig::VIEW_HEIGHT);
    Camera::Instance().SetBounds(0, 0, static_cast<float>(tileMap.GetWidthPixels()),
                                 static_cast<float>(tileMap.GetHeightPixels()));
    Camera::Instance().SetPosition(playerController.GetCenterPosition().x, playerController.GetCenterPosition().y);

    lastFrameTime = timeGetTime();
    isGameRunning = true;

    return true;
}

void GameScene::Start()
{
    for (auto& e : EnemyManager::Instance().GetActiveEnemies())
    {
        e->Start();
    }

    playerController.Start();
}

void GameScene::Update(const float deltaTime)
{
    TimeManager::Instance().Update(deltaTime);
    Camera::Instance().UpdateShake(deltaTime);
    Camera::Instance().UpdateLetterbox(deltaTime);

    if (TimeManager::Instance().IsHitStopped())
    {
        return;
    }

    // enemies / other objects will use this delta time
    const float worldDelta = TimeManager::Instance().GetWorldDeltaTime(deltaTime);


    for (const auto& e : EnemyManager::Instance().GetActiveEnemies())
    {
        e->Update(worldDelta, deltaTime, tileMap);
    }

    playerController.Update(deltaTime, tileMap);

    TriggerManager::Instance().Update(playerController, deltaTime);
    AttackVFXManager::Instance().Update(deltaTime);

    Vector2 camTarget{};
    camTarget.x = playerController.GetCenterPosition().x;
    camTarget.y = playerController.GetCenterPosition().y;
    Camera::Instance().FollowTarget(camTarget, 0.1f);

    TrackPlayerStatus();
}

void GameScene::Shutdown()
{
    tileMap.Shutdown();
    EnemyManager::Instance().Reset();
    TriggerManager::Instance().Clear();
}

void GameScene::Draw()
{
    ClearScreen();
    tileMap.Draw(Camera::Instance());

    // TriggerManager::Instance().DebugDraw(Camera::Instance());

    playerController.Draw(Camera::Instance());

    for (const auto& e : EnemyManager::Instance().GetActiveEnemies())
    {
        e->Draw(Camera::Instance());
    }

    if (TimeManager::Instance().IsTimeStopped())
    {
        VFXManager::ApplyGrayScaleToFrameBuffer();

        playerController.Draw(Camera::Instance());
    }

    Vector2 playerTopLeft = playerController.transform.topLeft;
    Vector2 playerCenter = playerController.transform.center;

    // char posBuf[128];
    // sprintf_s(posBuf, "Player Coords TOP LEFT| X: %f, Y %f\n"
    //           "CENTER| X: %f, Y %f", playerTopLeft.x, playerTopLeft.y, playerCenter.x, playerCenter.y);
    // WriteText(20, 60, posBuf, 10);

    AttackVFXManager::Instance().Draw(Camera::Instance());

    int barH = Camera::Instance().GetLetterboxHeight();
    if (barH > 0)
    {
        DrawRect(0, 0, GameConfig::VIEW_WIDTH, barH, BLACK, true); // top bar
        DrawRect(0, GameConfig::VIEW_HEIGHT - barH, GameConfig::VIEW_WIDTH, GameConfig::VIEW_HEIGHT, BLACK,
                 true); // bottom bar
    }

    PrintFrameBuffer();
    FlipScreen();
}

void GameScene::LoadTileset(const char* filePath)
{
    Bmp* tilesetImage = LoadBmp(filePath);

    // Check 1: Did the file load?
    if (tilesetImage == nullptr)
    {
        DebugPrintf("ERROR: Failed to load tileset from: %s\n", filePath);
        return;
    }

    DebugPrintf("Tileset loaded: %dx%d, %d-bit\n",
                tilesetImage->width, tilesetImage->height, tilesetImage->colbit);

    tileset.LoadTileset(tilesetImage, 32, 32);

    DebugPrintf("Tiles extracted: %d\n", static_cast<int>(tileset.tiles.size()));
}

void GameScene::TrackPlayerStatus()
{
    if (!GameManager::Instance().GetActivePlayer())
    {
        GameManager::Instance().SetIsVictory(false);
        RequestTransition(SceneID::GAMEOVER);
    }

    if (GameManager::Instance().GetIsVictory())
    {
        RequestTransition(SceneID::GAMEOVER);
    }
}
