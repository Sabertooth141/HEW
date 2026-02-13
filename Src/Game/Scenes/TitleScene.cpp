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

    if (ChkKeyEdge(PK_UP))
    {
        selectedOptionIndex -= 1;
        if (selectedOptionIndex < 0)
        {
            selectedOptionIndex = 0;
        }
    }

    if (ChkKeyEdge(PK_DOWN))
    {
        selectedOptionIndex += 1;
        if (selectedOptionIndex >= titleOptions.size())
        {
            selectedOptionIndex = titleOptions.size() - 1;
        }
    }

    if (ChkKeyEdge(PK_ENTER))
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

    if (ChkKeyEdge(PK_BS))
    {
        currTitleState = TitleStates::TITLE;
    }
}

void TitleScene::InitTitleAndOptions()
{
    const wchar_t* titleString = L"なんとかタイトル";
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
                option.onSelected = [this] { GoToGame(); };
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
    WriteTextW(static_cast<int>(title.transform.topLeft.x), static_cast<int>(title.transform.topLeft.y), title.text,
               title.fontSize);

    for (int i = 0; i < titleOptions.size(); i++)
    {
        Vector2 optionPos = titleOptions[i].transform.topLeft;
        D2D1::ColorF optionColor = D2D1::ColorF(0, 0, 0, 0);
        if (i == selectedOptionIndex)
        {
            optionColor = D2D1::ColorF::Red;
        }
        else
        {
            optionColor = D2D1::ColorF::White;
        }

        WriteTextW(optionPos.x, optionPos.y, titleOptions[i].text, titleOptions[i].fontSize, optionColor,
                   D2D1::ColorF(0, 0, 0, 0));
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

    tutorialTitle.text = titleString;
    tutorialTitle.fontSize = titleSize;
    tutorialTitle.transform = titleTransform;

    const wchar_t* tutorialString = L"A, D - 左右移動\n"
        "W, Space - ジャンプ\n"
        "Shift - ダッシュ\n"
        "K - 時間停止\n"
        "L - 時間戻り\n\n\n\n"
        "Backspace - タイトル画面へ";
    constexpr int tutorialSize = 12;
    Transform tutorialTransform{};
    tutorialTransform.size.x = static_cast<float>(CalculateTextWidth(tutorialString, tutorialSize));
    tutorialTransform.size.y = tutorialSize;

    Vector2 tutorialPos{};
    tutorialPos.x = static_cast<float>(viewWidth) / 2;
    tutorialPos.y = static_cast<float>(viewHeight) / 2 + 20;
    tutorialTransform.topLeft = Transform::ToTopLeft(tutorialPos, titleTransform.size);
    tutorialTransform.CalculateCenterPosition();

    tutorialText.text = tutorialString;
    tutorialText.fontSize = tutorialSize;
    tutorialText.transform = tutorialTransform;
}

void TitleScene::DrawTutorial()
{
    WriteTextW(tutorialTitle.transform.topLeft.x, tutorialTitle.transform.topLeft.y, tutorialTitle.text,
               tutorialTitle.fontSize);
    WriteTextW(tutorialText.transform.topLeft.x, tutorialText.transform.topLeft.y, tutorialText.text,
               tutorialText.fontSize);
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
