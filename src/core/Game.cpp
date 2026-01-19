//
// Created by saber on 1/14/2026.
//

#include "Game.h"

#include "../lib/conioex.h"
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

Game::Game() : isGameRunning(false), cam(), tileMap(), playerController(), lastFrameTime(0)
{
}

bool Game::Initialize()
{
    InitConioEx(VIEW_WIDTH, VIEW_HEIGHT, FONT_WIDTH, FONT_HEIGHT, true);
    SetCursorType(NOCURSOR);
    SetCaption("HEW PROTOTYPE");

    cam.Initialize(VIEW_WIDTH / 2, VIEW_HEIGHT / 2, VIEW_WIDTH, VIEW_HEIGHT);

    LoadTestLevel();

    float startX = 5 * TILE_SIZE;
    float startY = (TEST_MAP_HEIGHT - 6) * TILE_SIZE;
    // playerController
}

void Game::Start()
{
}

void Game::Update(float deltaTime)
{
}

void Game::ShutDown()
{
}

void Game::Draw()
{
}

void Game::HandleGlobalInput()
{
}

void Game::LoadTestLevel()
{
}

int main()
{
    Game game;
}
