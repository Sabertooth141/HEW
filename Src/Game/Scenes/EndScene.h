//
// Created by saber on 2/13/2026.
//

#ifndef HEW_ENDSCENE_H
#define HEW_ENDSCENE_H
#include "../Scene.h"
#include "../../Config/Structs.h"


class EndScene final : public Scene
{
public:
    bool Initialize() override;
    void HandleOptionInput();
    void Update(float deltaTime) override;
    void Start() override;
    void Draw() override;
    void Shutdown() override;
    void QuitGame();
    void GoToMenu();

private:
    TitleText endSceneTitle{};
    std::vector<TitleText> endSceneOptions{};

    int viewWidth = 0;
    int viewHeight = 0;
    int selectedOptionIndex = 0;
};


#endif //HEW_ENDSCENE_H
