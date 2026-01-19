//
// Created by saber on 1/14/2026.
//

#ifndef HEW_GAME_H
#define HEW_GAME_H

#define VIEW_WIDTH 800
#define VIEW_HEIGHT 600
#define FONT_WIDTH 1
#define FONT_HEIGHT 1
#define TILE_SIZE 16
#include "../Object/Entity/Player/PlayerController.h"
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

    Camera cam;
    Tilemap tileMap;
    PlayerController playerController;
    DWORD lastFrameTime;
};


#endif //HEW_GAME_H