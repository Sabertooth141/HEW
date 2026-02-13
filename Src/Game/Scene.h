//
// Created by saber on 2/13/2026.
//

#ifndef HEW_SCENE_H
#define HEW_SCENE_H
#include <cstdint>

enum class SceneID : uint8_t
{
    TITLE = 0,
    GAMEPLAY,
    GAMEOVER,
    DEFAULT
};

class Scene
{
public:
    virtual ~Scene() = default;

    virtual bool Initialize() = 0;
    virtual void Start() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;
    virtual void Shutdown() = 0;

    [[nodiscard]] SceneID GetNextScene() const { return nextScene; }
    [[nodiscard]] bool CanTransit() const { return nextScene != SceneID::DEFAULT;}

protected:
    void RequestTransition(const SceneID inNextScene) { nextScene = inNextScene; }

private:
    SceneID nextScene = SceneID::DEFAULT;

};


#endif //HEW_SCENE_H