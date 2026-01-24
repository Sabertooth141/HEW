//
// Created by saber on 1/14/2026.
//

#ifndef HEW_GAME_H
#define HEW_GAME_H

#define VIEW_WIDTH 400
#define VIEW_HEIGHT 300
#define FONT_WIDTH 2
#define FONT_HEIGHT 2
#define TILE_SIZE 16

#define TARGET_FPS 60
#define FRAME_TIME (1.0f / TARGET_FPS)

#include <memory>
#include <vector>

#include "../Object/Entity/Enemy/Enemy.h"
#include "../Object/Entity/Player/PlayerController.h"
#include "../VFX/VFXManager.h"
#include "../World/Camera.h"
#include "../World/Tilemap.h"

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

    // FOR TESTING ONLY
    void LoadTestLevel();

    // vars
private:
    bool isGameRunning = false;

    Camera cam{};
    Tilemap tileMap{};
    PlayerController playerController{};

    VFXManager vfxManager{};

    DWORD lastFrameTime = 0;

    GlobalInputConfig globalInputConfig;
};


#endif //HEW_GAME_H