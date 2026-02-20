//
// Created by saber on 1/14/2026.
//

#ifndef HEW_GAMESCENE_H
#define HEW_GAMESCENE_H

#include "../../Object/Entity/Enemy/Enemy.h"
#include "../../Object/Entity/Player/PlayerController.h"
#include "../Scene.h"
#include "../../VFX/VFXManager.h"
#include "../../World/Tilemap.h"
#include "../../World/Tileset.h"

struct PlayerNormalAnimPaths;

class GameScene final : public Scene
{
    // functions
public:
    GameScene();
    bool Initialize() override;
    void Start() override;
    void Shutdown() override;
    void Update(float deltaTime) override;
    void Draw() override;

private:
    void LoadTileset(const char* filePath);
    void TrackPlayerStatus();

    // vars
private:
    bool isGameRunning = false;

    Tilemap tileMap{};
    Tileset tileset{};
    PlayerController playerController{};

    VFXManager vfxManager{};

    DWORD lastFrameTime = 0;

    int* bgm = nullptr;
};

#endif //HEW_GAME_H