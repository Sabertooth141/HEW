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
    HandleInput();

    DebugPrintf("CURRENT INDEX: %d\n", selectedOptionIndex);
}

void TitleScene::Start()
{
    const wchar_t* titleString = L"なんとかタイトル";
    constexpr int titleSize = 40;
    Transform titleTransform;
    titleTransform.size.x = CalculateTextWidth(titleString, titleSize);
    titleTransform.size.y = titleSize;

    Vector2 titlePos;
    titlePos.x = viewWidth / 2;
    titlePos.y = viewHeight / 2 - 50;
    titleTransform.topLeft = Transform::ToTopLeft(titlePos, titleTransform.size);
    titleTransform.CalculateCenterPosition();

    title.text = titleString;
    title.fontSize = titleSize;
    title.transform = titleTransform;

    titleOptions.assign(3, TitleText{});
    for (int i = 0; i < 3; i++)
    {
        const wchar_t* optionString;
        switch (i)
        {
        case 0:
            {
                optionString = L"スタート";
                break;
            }
        case 1:
            {
                optionString = L"ヘルプ";
                break;
            }
        case 2:
            {
                optionString = L"ゲーム終了";
                break;
            }
        default:
            {
                optionString = L"";
                break;
            }
        }

        constexpr int optionSize = 14;
        Transform optionTransform;
        optionTransform.size.x = CalculateTextWidth(optionString, optionSize);
        optionTransform.size.y = optionSize;

        Vector2 optionPos;
        optionPos.x = viewWidth / 2;
        optionPos.y = viewHeight / 2 + 20 * i;
        optionTransform.topLeft = Transform::ToTopLeft(optionPos, optionTransform.size);
        optionTransform.CalculateCenterPosition();

        TitleText option;
        option.transform = optionTransform;
        option.text = optionString;
        option.fontSize = optionSize;

        titleOptions[i] = option;
    }
}

void TitleScene::Draw()
{
    ClearScreen();
    WriteTextW(title.transform.topLeft.x, title.transform.topLeft.y, title.text, title.fontSize);

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

    PrintFrameBuffer();
    FlipScreen();
}

void TitleScene::Shutdown()
{
}

void TitleScene::HandleInput()
{
    if (titleOptions.empty())
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
}

int TitleScene::CalculateTextWidth(const wchar_t* text, const int fontSize)
{
    int width = 0;
    for (int i = 0; text[i] != L'\0'; i++)
    {
        if (text[i] > 0x7F)
        {
            width += fontSize;
        }
        else
        {
            width += fontSize / 2;
        }
    }

    return width;
}
