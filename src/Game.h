//
// Created by saber on 1/14/2026.
//

#ifndef HEW_GAME_H
#define HEW_GAME_H

#define VIEW_WIDTH 800
#define VIEW_HEIGHT 600
#define FONT_WIDTH 1
#define FONT_HEIGHT 1
#include "World/Camera.h"
#include "World/Scene.h"

class Game
{
    // functions
public:
    void Initialize();
    void Start();
    void Update();
    void Draw();
    void ShutDown();

    // vars
private:
    bool isGameRunning = false;

    Camera cam;
    Scene scene;
};


#endif //HEW_GAME_H