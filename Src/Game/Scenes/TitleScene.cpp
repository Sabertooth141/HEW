//
// Created by saber on 2/13/2026.
//

#include "TitleScene.h"

#include "../Game.h"
#include "../../Lib/conioex_custom.h"

bool TitleScene::Initialize()
{
    viewWidth = GameConfig::VIEW_WIDTH;
    viewHeight = GameConfig::VIEW_HEIGHT;
    tutorialPage = 0;

    return true;
}

void TitleScene::Update(float deltaTime)
{
    HandleOptionInput();
    HandleTutorialInput();
}

void TitleScene::Start()
{
    InitTitleAndOptions();
    InitTutorial();

    currTitleState = TitleStates::TITLE;
}

void TitleScene::Draw()
{
    ClearScreen();

    if (currTitleState == TitleStates::TITLE)
    {
        DrawTitleAndOptions();
    }

    if (currTitleState == TitleStates::TUTORIAL)
    {
        DrawTutorial();
    }

    PrintFrameBuffer();
    FlipScreen();
}

void TitleScene::Shutdown()
{
    titleOptions.clear();
}

void TitleScene::HandleOptionInput()
{
    if (titleOptions.empty())
    {
        return;
    }

    if (currTitleState != TitleStates::TITLE)
    {
        return;
    }

    if (globalInputConfig.selectUp.IsEdge())
    {
        selectedOptionIndex -= 1;
        if (selectedOptionIndex < 0)
        {
            selectedOptionIndex = 0;
        }
    }

    if (globalInputConfig.selectDown.IsEdge())
    {
        selectedOptionIndex += 1;
        if (selectedOptionIndex >= titleOptions.size())
        {
            selectedOptionIndex = titleOptions.size() - 1;
        }
    }

    if (globalInputConfig.confirm.IsEdge())
    {
        titleOptions[selectedOptionIndex].onSelected();
    }
}

void TitleScene::HandleTutorialInput()
{
    if (currTitleState != TitleStates::TUTORIAL)
    {
        return;
    }

    if (globalInputConfig.back.IsEdge())
    {
        currTitleState = TitleStates::TITLE;
        tutorialPage = 0;
    }

    if (globalInputConfig.selectLeft.IsEdge())
    {
        if (tutorialPage > 0)
        {
            tutorialPage -= 1;
        }
    }

    if (globalInputConfig.selectRight.IsEdge())
    {
        if (tutorialPage < totalTutorialPage - 1)
        {
            tutorialPage += 1;
        }
    }

    if (tutorialPage == totalTutorialPage - 1)
    {
        if (globalInputConfig.confirm.IsEdge())
        {
            GoToGame();
        }
    }
}

void TitleScene::InitTitleAndOptions()
{
    const wchar_t* titleString = L"リワインドプロトコル";
    constexpr int titleSize = 40;
    Transform titleTransform{};
    titleTransform.size.x = static_cast<float>(CalculateTextWidth(titleString, titleSize));
    titleTransform.size.y = titleSize;

    Vector2 titlePos{};
    titlePos.x = static_cast<float>(viewWidth) / 2;
    titlePos.y = static_cast<float>(viewHeight) / 2 - 50;
    titleTransform.topLeft = Transform::ToTopLeft(titlePos, titleTransform.size);
    titleTransform.CalculateCenterPosition();

    title.text = titleString;
    title.fontSize = titleSize;
    title.transform = titleTransform;

    titleOptions.assign(3, TitleText{});
    for (int i = 0; i < 3; i++)
    {
        const wchar_t* optionString;
        TitleText option;

        switch (i)
        {
        case 0:
            {
                optionString = L"スタート";
                option.onSelected = [this]
                {
                    GoToTutorial();
                };
                break;
            }
        case 1:
            {
                optionString = L"チュートリアル";
                option.onSelected = [this] { GoToTutorial(); };
                break;
            }
        case 2:
            {
                optionString = L"ゲーム終了";
                option.onSelected = [this] { EndGame(); };
                break;
            }
        default:
            {
                optionString = L"";
                break;
            }
        }

        constexpr int optionSize = 14;
        Transform optionTransform{};
        optionTransform.size.x = static_cast<float>(CalculateTextWidth(optionString, optionSize));
        optionTransform.size.y = optionSize;

        Vector2 optionPos{};
        optionPos.x = static_cast<float>(viewWidth) / 2;
        optionPos.y = static_cast<float>(viewHeight) / 2 + 20.0f * i;
        optionTransform.topLeft = Transform::ToTopLeft(optionPos, optionTransform.size);
        optionTransform.CalculateCenterPosition();

        option.transform = optionTransform;
        option.text = optionString;
        option.fontSize = optionSize;

        titleOptions[i] = option;
    }
}

void TitleScene::DrawTitleAndOptions() const
{
    WriteTextW(
        static_cast<int>(title.transform.topLeft.x),
        static_cast<int>(title.transform.topLeft.y),
        title.text,
        title.fontSize
    );

    for (int i = 0; i < titleOptions.size(); i++)
    {
        Vector2 optionPos = titleOptions[i].transform.topLeft;
        D2D1::ColorF optionColor = (i == selectedOptionIndex)
                                       ? D2D1::ColorF(D2D1::ColorF::Red)
                                       : D2D1::ColorF(D2D1::ColorF::White);

        WriteTextW(
            static_cast<int>(optionPos.x),
            static_cast<int>(optionPos.y),
            titleOptions[i].text,
            titleOptions[i].fontSize,
            optionColor,
            D2D1::ColorF(0, 0, 0, 0)
        );
    }
}

void TitleScene::InitTutorial()
{
    const wchar_t* titleString = L"チュートリアル";
    constexpr int titleSize = 40;
    Transform titleTransform{};
    titleTransform.size.x = static_cast<float>(CalculateTextWidth(titleString, titleSize));
    titleTransform.size.y = titleSize;

    Vector2 titlePos{};
    titlePos.x = static_cast<float>(viewWidth) / 2;
    titlePos.y = static_cast<float>(viewHeight) / 2 - 50;
    titleTransform.topLeft = Transform::ToTopLeft(titlePos, titleTransform.size);
    titleTransform.CalculateCenterPosition();

    {
        Bmp* tutorial = LoadBmp("../Assets/TutorialAssets/Tutorial1.bmp");
        BmpImage tutorialImage{};
        tutorialImage.image = tutorial;
        tutorialImage.transform.center.x = static_cast<float>(viewWidth) / 2;
        tutorialImage.transform.center.y = static_cast<float>(viewHeight) / 2;
        tutorialImage.transform.size.x = tutorial->width;
        tutorialImage.transform.size.y = tutorial->height;
        tutorialImage.transform.CalculateTopLeftPosition();

        tutorialImages.push_back(tutorialImage);

        tutorial = LoadBmp("../Assets/TutorialAssets/Tutorial2.bmp");
        tutorialImage.image = tutorial;

        tutorialImages.push_back(tutorialImage);

        tutorial = LoadBmp("../Assets/TutorialAssets/Tutorial3.bmp");
        tutorialImage.image = tutorial;

        tutorialImages.push_back(tutorialImage);

        tutorial = LoadBmp("../Assets/TutorialAssets/Tutorial4.bmp");
        tutorialImage.image = tutorial;

        tutorialImages.push_back(tutorialImage);

        tutorial = LoadBmp("../Assets/TutorialAssets/Tutorial5.bmp");
        tutorialImage.image = tutorial;

        tutorialImages.push_back(tutorialImage);
    }

    totalTutorialPage = tutorialImages.size();
}

void TitleScene::DrawTutorial() const
{
    DrawBmp(tutorialImages[tutorialPage].transform.topLeft.x, tutorialImages[tutorialPage].transform.topLeft.y,
            tutorialImages[tutorialPage].image);
}

void TitleScene::GoToGame()
{
    RequestTransition(SceneID::GAMEPLAY);
}

void TitleScene::GoToTutorial()
{
    currTitleState = TitleStates::TUTORIAL;
}

void TitleScene::EndGame()
{
    isSceneRunning = false;
}
