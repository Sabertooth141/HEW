//
// Created by saber on 1/14/2026.
//

#include "Game.h"

#include <format>

#include "../Config/EntityConfigs.h"
#include "../Config/SystemConfigs.h"
#include "../Lib/conioex.h"
#include "../Systems/EnemyManager.h"
#include "../Systems/TimeManager.h"
#include "../World/Tileset.h"
#define TEST_MAP_WIDTH 100
#define TEST_MAP_HEIGHT 50

int GameConfig::VIEW_WIDTH = 400;
int GameConfig::VIEW_HEIGHT = 300;
int GameConfig::FONT_WIDTH = 2;
int GameConfig::FONT_HEIGHT = 2;

void GameConfig::CalculateViewportSize()
{
    // Get ACTUAL screen resolution (ignoring Windows scaling)
    DEVMODE dm;
    dm.dmSize = sizeof(DEVMODE);
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);

    int screenW = dm.dmPelsWidth;   // Real resolution
    int screenH = dm.dmPelsHeight;  // Real resolution

    // Also get DPI scaling factor
    HDC hdc = GetDC(NULL);
    int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
    int dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
    ReleaseDC(NULL, hdc);

    float dpiScale = dpiX / 96.0f;  // 96 DPI = 100% scaling

    char debug[512];
    sprintf_s(debug,
        "Real Screen: %dx%d | DPI Scale: %.2f (%.0f%%) | Scaled: %dx%d\n",
        screenW, screenH, dpiScale, dpiScale * 100.0f,
        GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    OutputDebugString(debug);

    // Use REAL resolution for calculations
    const int BASELINE_SCREEN_WIDTH = 2880;
    const int BASELINE_SCREEN_HEIGHT = 1620;

    // Adjust font based on REAL resolution
    if (screenW >= 2880) {
        FONT_WIDTH = 2;
        FONT_HEIGHT = 2;
    }
    else if (screenW >= 1920) {
        // 1080p - use 3x3 for better fill
        FONT_WIDTH = 3;
        FONT_HEIGHT = 3;
    }
    else if (screenW >= 1600) {
        FONT_WIDTH = 3;
        FONT_HEIGHT = 3;
    }
    else {
        FONT_WIDTH = 4;
        FONT_HEIGHT = 4;
    }

    int usableHeight = screenH - 100;
    int maxViewWidth = (screenW - 20) / FONT_WIDTH;
    int maxViewHeight = usableHeight / FONT_HEIGHT;

    float widthRatio = static_cast<float>(screenW) / static_cast<float>(BASELINE_SCREEN_WIDTH);
    float heightRatio = static_cast<float>(screenH) / static_cast<float>(BASELINE_SCREEN_HEIGHT);
    float scale = (widthRatio < heightRatio) ? widthRatio : heightRatio;

    VIEW_WIDTH = static_cast<int>(400 * scale);
    VIEW_HEIGHT = static_cast<int>(300 * scale);

    if (VIEW_WIDTH > maxViewWidth) VIEW_WIDTH = maxViewWidth;
    if (VIEW_HEIGHT > maxViewHeight) VIEW_HEIGHT = maxViewHeight;

    // Safety clamps
    if (VIEW_WIDTH < 160) VIEW_WIDTH = 160;
    if (VIEW_HEIGHT < 120) VIEW_HEIGHT = 120;
    if (VIEW_WIDTH > 600) VIEW_WIDTH = 600;
    if (VIEW_HEIGHT > 450) VIEW_HEIGHT = 450;

    sprintf_s(debug,
        "Final Config: View=%dx%d tiles | Font=%dx%d | Physical=%dx%d px | Scale=%.2f\n",
        VIEW_WIDTH, VIEW_HEIGHT,
        FONT_WIDTH, FONT_HEIGHT,
        VIEW_WIDTH * FONT_WIDTH, VIEW_HEIGHT * FONT_HEIGHT,
        scale);
    OutputDebugString(debug);
}

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
    GameConfig::CalculateViewportSize();

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
    constexpr float playerStartY = (TEST_MAP_HEIGHT - 6) * TILE_SIZE;

    // playerController
    PlayerConfig playerCfg = config::Player();
    playerCfg.x = playerStartX;
    playerCfg.y = playerStartY;

    playerController.Initialize(playerCfg);

    // TODO: TEMP
    constexpr float enemyStartX = playerStartX + 20;
    constexpr float enemyStartY = playerStartY;

    // enemies
    EnemyConfig enemyCfg = config::Enemy();
    enemyCfg.x = enemyStartX;
    enemyCfg.y = enemyStartY;

    EnemyManager::Instance().CreateEnemy(enemyCfg);

    // subsystems
    const VFXConfig vfxCfg = sysCfg::VFXCfg();
    vfxManager.Initialize(vfxCfg);

    const TimeManagerConfig timeManagerCfg = sysCfg::TimeCfg();
    TimeManager::Instance().Initialize(timeManagerCfg);

    // camera
    cam.Initialize(GameConfig::VIEW_WIDTH / 2, GameConfig::VIEW_HEIGHT / 2, GameConfig::VIEW_WIDTH, GameConfig::VIEW_HEIGHT);
    cam.SetBounds(0, 0, tileMap.GetWidthPixels(), tileMap.GetHeightPixels());
    cam.SetPosition(playerController.GetCenterX(), playerController.GetCenterY());

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

    while (isGameRunning)
    {
        const DWORD currTime = timeGetTime();
        float deltaTime = static_cast<float>(currTime - lastFrameTime) / 1000.0f;

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
    const float worldDelta = TimeManager::Instance().GetWorldDeltaTime(deltaTime);


    for (const auto& e : EnemyManager::Instance().GetActiveEnemies())
    {
        e->Update(worldDelta, tileMap);
    }

    playerController.Update(deltaTime, tileMap);

    TargetPosition camTarget{};
    camTarget.x = playerController.GetCenterX();
    camTarget.y = playerController.GetCenterY();
    cam.FollowTarget(camTarget, 0.1f);
}

void Game::ShutDown()
{
    tileMap.Shutdown();
    EndConioEx();
}

void Game::Draw()
{
    if (TimeManager::Instance().IsTimeStopped())
    {
        vfxManager.ApplyGrayscale();
    }
    else
    {
        vfxManager.ApplyNormalPal();
    }

    ClearFrameBuffer();
    tileMap.Draw(cam);

    // Bmp* testTileset = LoadBmp("../Assets/Tileset/SceneTileset/anim-0.bmp");
    // DrawBmp(5, 5, testTileset, true);

    // LoadTileset("../Assets/Tileset/SceneTileset/TestTileset.bmp");

    // Check before drawing
    // Bmp* testTile = tileset.GetTile(5);
    // if (testTile != nullptr)
    // {
    //     DrawBmp(5 + 10, 1, testTile, true);
    // }

    COLORREF palette[16];
    GetCurrentPalette(palette);

    for (const auto& e : EnemyManager::Instance().GetActiveEnemies())
    {
        e->Draw(cam);
    }
    // vfxManager.ApplyNormalPal();

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

void Game::LoadTileset(const char* filePath)
{
    Bmp* tilesetImage = LoadBmp(filePath, true);

    // Check 1: Did the file load?
    if (tilesetImage == nullptr)
    {
        printf("ERROR: Failed to load tileset from: %s\n", filePath);
        return;
    }

    printf("Tileset loaded: %dx%d, %d-bit\n",
           tilesetImage->width, tilesetImage->height, tilesetImage->colbit);

    tileset.LoadTileset(tilesetImage, 32, 32);

    // Check 2: How many tiles were created?
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
    Game game;
    if (!game.Initialize())
    {
        return -1;
    }
    game.Start();
    game.ShutDown();

    return 0;
}
