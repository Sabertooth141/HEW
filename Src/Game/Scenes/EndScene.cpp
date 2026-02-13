//
// Created by saber on 2/13/2026.
//

#include "EndScene.h"

#include "../Game.h"
#include "../../Systems/GameManager.h"
#include "../../Lib/conioex_custom.h"

bool EndScene::Initialize()
{
    viewWidth = GameConfig::VIEW_WIDTH;
    viewHeight = GameConfig::VIEW_HEIGHT;
    return true;
}

void EndScene::Update(float deltaTime)
{
}

void EndScene::Start()
{
    if (GameManager::Instance().GetIsVictory())
    {
        const wchar_t* titleString = L"勝ったぞー";
        constexpr int titleSize = 40;
        Transform titleTransform{};
        titleTransform.size.x = static_cast<float>(CalculateTextWidth(titleString, titleSize));
        titleTransform.size.y = titleSize;

        Vector2 titlePos{};
        titlePos.x = static_cast<float>(viewWidth) / 2;
        titlePos.y = static_cast<float>(viewHeight) / 2 - 50;
        titleTransform.topLeft = Transform::ToTopLeft(titlePos, titleTransform.size);
        titleTransform.CalculateCenterPosition();

        endSceneTitle.text = titleString;
        endSceneTitle.fontSize = titleSize;
        endSceneTitle.transform = titleTransform;
    }
    else
    {
        const wchar_t* titleString = L"死んだぞー";
        constexpr int titleSize = 40;
        Transform titleTransform{};
        titleTransform.size.x = static_cast<float>(CalculateTextWidth(titleString, titleSize));
        titleTransform.size.y = titleSize;

        Vector2 titlePos{};
        titlePos.x = static_cast<float>(viewWidth) / 2;
        titlePos.y = static_cast<float>(viewHeight) / 2 - 50;
        titleTransform.topLeft = Transform::ToTopLeft(titlePos, titleTransform.size);
        titleTransform.CalculateCenterPosition();

        endSceneTitle.text = titleString;
        endSceneTitle.fontSize = titleSize;
        endSceneTitle.transform = titleTransform;
    }

    endSceneOptions.assign(2, TitleText{});
    for (int i = 0; i < 2; ++i)
    {
        const wchar_t* optionString;
        TitleText option;

        switch (i)
        {
        case 0:
            {
                optionString = L"終了";
                option.onSelected = [this] { QuitGame(); };
                break;
            }
        case 1:
            {
                optionString = L"リトライ";
                option.onSelected = [this] { GoToMenu(); };
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

        endSceneOptions[i] = option;
    }
}

void EndScene::Draw()
{
}

void EndScene::Shutdown()
{
}

void EndScene::QuitGame()
{
    return;
}

void EndScene::GoToMenu()
{
    return;
}
