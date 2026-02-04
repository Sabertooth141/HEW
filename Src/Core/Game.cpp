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

#define TEST_MAP_WIDTH 100
#define TEST_MAP_HEIGHT 50

int GameConfig::VIEW_WIDTH = 500;
int GameConfig::VIEW_HEIGHT = 300;
int GameConfig::FONT_WIDTH = 2;
int GameConfig::FONT_HEIGHT = 2;

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

static unsigned char testMapData[TEST_MAP_WIDTH * TEST_MAP_HEIGHT];

static void GenerateTestLevel()
{
    for (int i = 0; i < TEST_MAP_WIDTH * TEST_MAP_HEIGHT; i++)
    {
        testMapData[i] = 0; // AIR
    }

    for (int x = 0; x < TEST_MAP_WIDTH; x++)
    {
        for (int y = TEST_MAP_HEIGHT - 3; y < TEST_MAP_HEIGHT; y++)
        {
            testMapData[y * TEST_MAP_WIDTH + x] = 1; // SOLID
        }
    }

    // PLATFORMS
    for (int x = 10; x < 18; x++)
    {
        testMapData[(TEST_MAP_HEIGHT - 8) * TEST_MAP_WIDTH + x] = 2;
    }

    for (int x = 22; x < 32; x++)
    {
        testMapData[(TEST_MAP_HEIGHT - 12) * TEST_MAP_WIDTH + x] = 2;
    }

    // WALLS
    for (int y = TEST_MAP_HEIGHT - 8; y < TEST_MAP_HEIGHT - 3; y++)
    {
        testMapData[y * TEST_MAP_WIDTH + 50] = 1;
        testMapData[y * TEST_MAP_WIDTH + 51] = 1;
    }

    for (int y = 0; y < TEST_MAP_HEIGHT; y++)
    {
        testMapData[y * TEST_MAP_WIDTH + 0] = 1;
    }

    for (int y = 0; y < TEST_MAP_HEIGHT; y++)
    {
        testMapData[y * TEST_MAP_WIDTH + (TEST_MAP_WIDTH - 1)] = 1;
    }
}

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

    LoadTestLevel();

    // TODO: TEMP
    constexpr float playerStartX = 5 * TILE_SIZE;
    constexpr float playerStartY = (TEST_MAP_HEIGHT - 16) * TILE_SIZE;

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

    // TODO: TEMP
    constexpr float enemyStartX = playerStartX + 20;
    constexpr float enemyStartY = playerStartY;

    // enemies
    // EnemyConfig enemyCfg = config::Enemy();
    // enemyCfg.x = enemyStartX;
    // enemyCfg.y = enemyStartY;
    // enemyCfg.targetTransform = &playerController.transform;
    //
    // EnemyManager::Instance().CreateEnemy(enemyCfg);

    MineConfig mineCfg = config::Mine();
    mineCfg.x = enemyStartX + 50;
    mineCfg.y = enemyStartY;
    mineCfg.targetTransform = &playerController.transform;

    EnemyManager::Instance().CreateMine(mineCfg);

    // subsystems
    const VFXConfig vfxCfg = sysCfg::VFXCfg();
    vfxManager.Initialize(vfxCfg);

    const TimeManagerConfig timeManagerCfg = sysCfg::TimeCfg();
    TimeManager::Instance().Initialize(timeManagerCfg);

    // camera
    cam.Initialize(GameConfig::VIEW_WIDTH / 2, GameConfig::VIEW_HEIGHT / 2, GameConfig::VIEW_WIDTH,
                   GameConfig::VIEW_HEIGHT);
    cam.SetBounds(0, 0, tileMap.GetWidthPixels(), tileMap.GetHeightPixels());
    cam.SetPosition(playerController.GetCenterPosition().x, playerController.GetCenterPosition().y);

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

    // enemies / other objects will use this delta time
    const float worldDelta = TimeManager::Instance().GetWorldDeltaTime(deltaTime);


    for (const auto& e : EnemyManager::Instance().GetActiveEnemies())
    {
        e->Update(worldDelta, tileMap);
    }

    playerController.Update(deltaTime, tileMap);

    Vector2 camTarget{};
    camTarget.x = playerController.GetCenterPosition().x;
    camTarget.y = playerController.GetCenterPosition().y;
    cam.FollowTarget(camTarget, 0.1f);
}

void Game::ShutDown()
{
    tileMap.Shutdown();
    EndConioEx();
}

void Game::Draw()
{

    ClearScreen();
    tileMap.Draw(cam);

    playerController.Draw(cam);

    for (const auto& e : EnemyManager::Instance().GetActiveEnemies())
    {
        e->Draw(cam);
    }

    if (TimeManager::Instance().IsTimeStopped())
    {
        DebugPrintf("time stopped\n");
        vfxManager.ApplyGrayScaleToFrameBuffer();

        playerController.Draw(cam);
    }

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
        printf("ERROR: Failed to load tileset from: %s\n", filePath);
        return;
    }

    printf("Tileset loaded: %dx%d, %d-bit\n",
           tilesetImage->width, tilesetImage->height, tilesetImage->colbit);

    tileset.LoadTileset(tilesetImage, 32, 32);

    printf("Tiles extracted: %d\n", (int)tileset.tiles.size());
}

void Game::LoadTestLevel()
{
    GenerateTestLevel();
    tileMap.Initialize(TEST_MAP_WIDTH, TEST_MAP_HEIGHT, TILE_SIZE);
    tileMap.LoadFromArr(testMapData, TEST_MAP_WIDTH, TEST_MAP_HEIGHT);
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
