//
// Created by saber on 2/13/2026.
//

#include "../Game/SceneManager.h"

#include "Scenes/EndScene.h"
#include "Scenes/GameScene.h"
#include "Scenes/TitleScene.h"

void SceneManager::SetScene(const SceneID id)
{
    if (currentScene)
    {
        currentScene->Shutdown();
    }
    currentScene = CreateScene(id);

    if (currentScene)
    {
        currentScene->Initialize();
        currentScene->Start();
    }
}

void SceneManager::Update(const float deltaTime)
{
    if (currentScene == nullptr)
    {
        return;
    }

    currentScene->Update(deltaTime);
    currentScene->Draw();

    if (currentScene->CanTransit())
    {
        SetScene(currentScene->GetNextScene());
    }
}

std::unique_ptr<Scene> SceneManager::CreateScene(const SceneID id)
{
    // TODO: ADD OTHER SCENES
    switch (id)
    {
    case SceneID::GAMEPLAY:
        {
            return std::make_unique<GameScene>();
        }
    case SceneID::TITLE:
        {
            return std::make_unique<TitleScene>();
        }
    case SceneID::GAMEOVER:
        {
            return std::make_unique<EndScene>();
        }
    default:
        {
            return nullptr;
        }
    }
}
