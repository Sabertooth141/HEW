//
// Created by saber on 1/14/2026.
//

#include "Game.h"

#include "../Config/EntityConfigs.h"
#include "../Config/SystemConfigs.h"
#include "../Lib/conioex.h"
#include "../Systems/TimeManager.h"
#define TEST_MAP_WIDTH 100
#define TEST_MAP_HEIGHT 50

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
    InitConioEx(VIEW_WIDTH, VIEW_HEIGHT, FONT_WIDTH, FONT_HEIGHT, true);
    SetCursorType(NOCURSOR);
    SetCaption("HEW PROTOTYPE");

    cam.Initialize(VIEW_WIDTH / 2, VIEW_HEIGHT / 2, VIEW_WIDTH, VIEW_HEIGHT);

    LoadTestLevel();

    constexpr float startX = 5 * TILE_SIZE;
    constexpr float startY = (TEST_MAP_HEIGHT - 6) * TILE_SIZE;
    // playerController

    PlayerConfig playerCfg = config::Player();
    playerCfg.x = startX;
    playerCfg.y = startY;
    playerController.Initialize(playerCfg);

    const VFXConfig vfxCfg = sysCfg::VFX();
    vfx.Initialize(vfxCfg);

    cam.SetBounds(0, 0, tileMap.GetWidthPixels(), tileMap.GetHeightPixels());
    cam.SetPosition(playerController.GetCenterX(), playerController.GetCenterY());

    lastFrameTime = timeGetTime();
    isGameRunning = true;

    return true;
}

void Game::Start()
{
    playerController.Start();

    while (isGameRunning)
    {
        DWORD currTime = timeGetTime();
        float deltaTime = (currTime - lastFrameTime) / 1000.0f;

        if (deltaTime > 0.1f)
        {
            deltaTime = 0.1f;
        }

        if (deltaTime >= FRAME_TIME)
        {
            lastFrameTime = currTime;
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
    float worldDelta = TimeManager::Instance().GetWorldDeltaTime(deltaTime);

    playerController.Update(deltaTime, tileMap);
    cam.FollowTarget(playerController.GetCenterX(), playerController.GetCenterY(), 0.1f);
}

void Game::ShutDown()
{
    tileMap.Shutdown();
    EndConioEx();
}

void Game::Draw()
{
    // TODO: NEED TO FIX THIS
    // if (TimeManager::Instance().IsTimeStopped())
    // {
    //     vfx.ApplyGrayscale();
    // }
    // else
    // {
    //     vfx.ApplyNormalPal();
    // }

    ClearFrameBuffer();
    tileMap.Draw(cam);
    playerController.Draw(cam);
    PrintFrameBuffer();
    FlipScreen();
}

void Game::HandleGlobalInput()
{
    GetKeyAll();

    if (globalInputConfig.quitGame.IsEdge())
    {
        isGameRunning = false;
    }
}

void Game::LoadTestLevel()
{
    GenerateTestLevel();
    tileMap.Initialize(TEST_MAP_WIDTH, TEST_MAP_HEIGHT, TILE_SIZE);
    tileMap.LoadFromArr(testMapData, TEST_MAP_WIDTH, TEST_MAP_HEIGHT);
}

int main()
{
    Game game;
    if (!game.Initialize())
    {
        return -1;
    }
    game.Start();
    game.ShutDown();

    return 0;
}
