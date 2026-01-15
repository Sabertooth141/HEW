//
// Created by saber on 1/15/2026.
//

#ifndef HEW_SCENE_H
#define HEW_SCENE_H
#include "Camera.h"
#include "Tileset.h"
#include "../Object/Entity/Player/PlayerController.h"

enum TimeState
{
    Present,
    Past
};

class Scene
{
    // functions
public:
    Scene();
    void Load();
    void Update();
    void Draw(const Camera& cam);

    // vars
public:
    TileMap mapPresent;
    TileMap mapPast;
    TileMap* activeMap = nullptr;

    Tileset tileset;

    PlayerController player;
    TimeState timeState = Present;
};

#endif //HEW_SCENE_H