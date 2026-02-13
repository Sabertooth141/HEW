//
// Created by saber on 2/13/2026.
//

#ifndef HEW_SCENEMANAGER_H
#define HEW_SCENEMANAGER_H
#include <memory>

#include "Scene.h"


class SceneManager
{
public:
    void SetScene(SceneID id);
    void Update(float deltaTime);
    bool HasScene() const { return currentScene != nullptr; }
    Scene* GetScene() const { return currentScene.get(); }

private:
    static std::unique_ptr<Scene> CreateScene(SceneID id);
    std::unique_ptr<Scene> currentScene;
};


#endif //HEW_SCENEMANAGER_H