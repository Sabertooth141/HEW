//
// Created by saber on 1/14/2026.
//

#ifndef HEW_GAME_H
#define HEW_GAME_H

//#define VIEW_WIDTH 400
//#define VIEW_HEIGHT 300
//#define FONT_WIDTH 2
//#define FONT_HEIGHT 2
#define TILE_SIZE 16

#define TARGET_FPS 60
#define FRAME_TIME (1.0f / TARGET_FPS)

struct GameConfig
{
    static int VIEW_WIDTH;
    static int VIEW_HEIGHT;
    static int FONT_WIDTH;
    static int FONT_HEIGHT;
};

#include <memory>
#include <string>
#include <vector>

#include "../Object/Entity/Enemy/Enemy.h"
#include "../Object/Entity/Player/PlayerController.h"
#include "../VFX/VFXManager.h"
#include "../World/Camera.h"
#include "../World/Tilemap.h"
#include "../World/Tileset.h"

class Game
{
    // functions
public:
    Game();
    bool Initialize();
    void Start();
    void ShutDown();

private:
    void Update(float deltaTime);
    void Draw();

    void HandleGlobalInput();
    void LoadTileset(const char* filePath);

    // FOR TESTING ONLY
    void LoadTestLevel();

    // vars
private:
    bool isGameRunning = false;

    Camera cam{};
    Tilemap tileMap{};
    Tileset tileset{};
    PlayerController playerController{};

    VFXManager vfxManager{};

    DWORD lastFrameTime = 0;

    GlobalInputConfig globalInputConfig;

    COLORREF GetCurrentPalette(COLORREF* outPalette)
    {
        CONSOLE_SCREEN_BUFFER_INFOEX csbi;
        csbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

        HANDLE hConsole = GetCurrentHandle();  // conioex function
        GetConsoleScreenBufferInfoEx(hConsole, &csbi);

        // Copy the 16 colors
        for (int i = 0; i < 16; i++)
        {
            outPalette[i] = csbi.ColorTable[i];
        }

        return outPalette[0];  // or return nothing
    }
};


#endif //HEW_GAME_H