//
// Created by saber on 1/14/2026.
//

#include "Game.h"

#include <format>

#include "../Animation/SpriteSheetLoader.h"
#include "../Config/EntityConfigs.h"
#include "../Config/SystemConfigs.h"
#include "../Lib/conioex_custom.h"
#include "../Systems/EnemyManager.h"
#include "../VFX/AttackVFXManager.h"
#include "../Object/Entity/Enemy/Mine/Mine.h"
#include "../Object/Entity/Enemy/UGV/UGV.h"

#define TEST_MAP_WIDTH 100
#define TEST_MAP_HEIGHT 50

int GameConfig::VIEW_WIDTH = 500;
int GameConfig::VIEW_HEIGHT = 300;
int GameConfig::FONT_WIDTH = 2;
int GameConfig::FONT_HEIGHT = 2;

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

// mine
std::vector<EnemyAnimPaths<EnemyVFXType>> mineAttackVFXPaths =
{
    {
        EnemyVFXType::MINE,
        "../Assets/Enemy/Mine/AttkVFX/MineAttkVFX.json",
        "../Assets/Enemy/Mine/AttkVFX/MineAttkVFX.bmp"
    }
};

// ========================================================================

Game::Game()
= default;

bool Game::Initialize()
{
    InitConioEx(GameConfig::VIEW_WIDTH, GameConfig::VIEW_HEIGHT, GameConfig::FONT_WIDTH, GameConfig::FONT_HEIGHT, true);
    SetCursorType(NOCURSOR);

    char title[128];
    sprintf_s(title, "HEW PROTOTYPE - %dx%d (Font: %dx%d)",
              GameConfig::VIEW_WIDTH, GameConfig::VIEW_HEIGHT,
              GameConfig::FONT_WIDTH, GameConfig::FONT_HEIGHT);
    SetCaption(title);

    // init tileMap
    LoadTileset("../Assets/Tileset/SceneTileset/Tileset.bmp");

    int mapWidth, mapHeight;
    std::vector<uint8_t> mapData = tileMap.ParseMapCSV("../Assets/Maps/MapCsv/TestMap.csv", mapWidth, mapHeight);
    tileMap.LoadFromArr(mapData, mapWidth, mapHeight, tileset, TILE_SIZE);

    // init vfxmanager
    for (const auto& effect : playerAttackVFXPaths)
    {
        AttackVFXManager::Instance().InitAnimation(effect);
    }

    for (const auto& effect : mineAttackVFXPaths)
    {
        AttackVFXManager::Instance().InitAnimation(effect);
    }

    // TODO: TEMP
    const float playerStartX = mapWidth / 2 * TILE_SIZE - 200;
    const float playerStartY = mapHeight / 2 * TILE_SIZE + 300;

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

    UGVConfig ugvCfg = config::UGV();
    ugvCfg.x = playerStartX;
    ugvCfg.y = playerStartY;
    ugvCfg.target = &playerController;

    EnemyManager::Instance().CreateEnemy<UGV, UGVConfig>(ugvCfg);

    // subsystems
    const VFXConfig vfxCfg = sysCfg::VFXCfg();
    vfxManager.Initialize(vfxCfg);

    const TimeManagerConfig timeManagerCfg = sysCfg::TimeCfg();
    TimeManager::Instance().Initialize(timeManagerCfg);

    // camera
    Camera::Instance().Initialize(static_cast<float>(GameConfig::VIEW_WIDTH) / 2, static_cast<float>(GameConfig::VIEW_HEIGHT) / 2,
                   GameConfig::VIEW_WIDTH,
                   GameConfig::VIEW_HEIGHT);
    Camera::Instance().SetBounds(0, 0, static_cast<float>(tileMap.GetWidthPixels()), static_cast<float>(tileMap.GetHeightPixels()));
    Camera::Instance().SetPosition(playerController.GetCenterPosition().x, playerController.GetCenterPosition().y);

    lastFrameTime = timeGetTime();
    isGameRunning = true;

    return true;
}

void Game::Start()
{
    for (auto& e : EnemyManager::Instance().GetActiveEnemies())
    {
        e->Start();
    }
    playerController.Start();

    vfxManager.SetNormalPal(SpriteSheetLoader::GetGamePalColor());
    while (isGameRunning)
    {
        const DWORD currTime = timeGetTime();
        float deltaTime = static_cast<float>(currTime - lastFrameTime) / 1000.0f;

        if (deltaTime > FRAME_TIME * 2)
        {
            deltaTime = FRAME_TIME * 2;
        }

        if (deltaTime >= FRAME_TIME)
        {
            lastFrameTime = currTime;
            GetKeyAll();
            HandleGlobalInput();
            Update(deltaTime);
            Draw();
        }
        else
        {
            Sleep(1);
        }
    }
}

void Game::Update(const float deltaTime)
{
    TimeManager::Instance().Update(deltaTime);
    Camera::Instance().UpdateShake(deltaTime);

    if (TimeManager::Instance().IsHitStopped())
    {
        return;
    }

    // enemies / other objects will use this delta time
    const float worldDelta = TimeManager::Instance().GetWorldDeltaTime(deltaTime);


    for (const auto& e : EnemyManager::Instance().GetActiveEnemies())
    {
        e->Update(worldDelta, tileMap);
    }

    playerController.Update(deltaTime, tileMap);

    AttackVFXManager::Instance().Update(deltaTime);

    Vector2 camTarget{};
    camTarget.x = playerController.GetCenterPosition().x;
    camTarget.y = playerController.GetCenterPosition().y;
    Camera::Instance().FollowTarget(camTarget, 0.1f);
}

void Game::ShutDown()
{
    tileMap.Shutdown();
    EndConioEx();
}

void Game::Draw()
{
    ClearScreen();
    tileMap.Draw(Camera::Instance());

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

    AttackVFXManager::Instance().Draw(Camera::Instance());

    WriteTextW(20, GameConfig::VIEW_HEIGHT - 30, L"テスト", 20);

    PrintFrameBuffer();
    FlipScreen();
}

void Game::HandleGlobalInput()
{
    if (globalInputConfig.quitGame.IsEdge())
    {
        isGameRunning = false;
    }
}

void Game::LoadTileset(const char* filePath)
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

    DebugPrintf("Tiles extracted: %d\n", (int)tileset.tiles.size());
}

int main()
{
    CheckConsoleCompatibility();

    Game game;
    if (!game.Initialize())
    {
        return -1;
    }
    game.Start();
    game.ShutDown();

    return 0;
}
