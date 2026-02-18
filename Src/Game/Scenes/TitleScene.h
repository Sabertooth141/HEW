//
// Created by saber on 2/13/2026.
//

#ifndef HEW_TITLESCENE_H
#define HEW_TITLESCENE_H

#include "../Scene.h"
#include "../../Config/Structs.h"

enum class TitleStates : uint8_t
{
    TITLE,
    TUTORIAL,
    START_TUTORIAL,
    DEFAULT
};

struct BmpImage
{
    Bmp* image;
    Transform transform;
};

class TitleScene final : public Scene
{
public:
    bool Initialize() override;
    void Update(float deltaTime) override;
    void Start() override;
    void Draw() override;
    void Shutdown() override;

private:
    void HandleOptionInput();
    void HandleTutorialInput();

    void InitTitleAndOptions();
    void DrawTitleAndOptions() const;

    void InitTutorial();
    void DrawTutorial() const;

    void GoToGame();
    void GoToTutorial();
    void EndGame();

private:
    std::vector<TitleText> titleOptions;
    TitleText title{};
    TitleStates currTitleState = TitleStates::DEFAULT;

    std::vector<BmpImage> tutorialImages{};

    int viewWidth = 0;
    int viewHeight = 0;
    int selectedOptionIndex = 0;

    int tutorialPage = 0;
    int totalTutorialPage = 1;
};


#endif //HEW_TITLESCENE_H