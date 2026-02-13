//
// Created by saber on 2/13/2026.
//

#include "Game/Game.h"
#include "Game/SceneManager.h"
#include "Game/Scenes/GameScene.h"

inline bool IsLegacyConsoleHost()
{
    const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFOEX csbiex;
    csbiex.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

    if (!GetConsoleScreenBufferInfoEx(hOut, &csbiex))
    {
        return false;
    }

    int validColors = 0;
    for (unsigned long i : csbiex.ColorTable)
    {
        if (i != 0)
        {
            validColors++;
        }
    }

    return validColors >= 16;
}

inline void CheckConsoleCompatibility()
{
    if (!IsLegacyConsoleHost())
    {
        MessageBoxA(nullptr,
                    "WARNING: This game requires Windows Console Host.\n\n"
                    "Windows Terminal detected - rendering may be incorrect.\n\n"
                    "To fix:\n"
                    "1. Open Windows Settings\n"
                    "2. Go to: Privacy & Security → For developers\n"
                    "3. Change 'Terminal' to 'Windows Console Host'\n"
                    "4. Restart this game\n\n"
                    "Press OK to continue anyway (not recommended)",
                    "Console Compatibility Warning",
                    MB_OK | MB_ICONWARNING);
    }
}

int main()
{
    CheckConsoleCompatibility();

    InitConioEx(GameConfig::VIEW_WIDTH, GameConfig::VIEW_HEIGHT, GameConfig::FONT_WIDTH, GameConfig::FONT_HEIGHT, true);
    SetCursorType(NOCURSOR);

    char title[128];
    sprintf_s(title, "HEW PROTOTYPE - %dx%d (Font: %dx%d)",
              GameConfig::VIEW_WIDTH, GameConfig::VIEW_HEIGHT,
              GameConfig::FONT_WIDTH, GameConfig::FONT_HEIGHT);
    SetCaption(title);

    SceneManager sceneManager;
    sceneManager.SetScene(SceneID::TITLE);

    DWORD lastFrameTime = timeGetTime();
    bool gameRunning = true;

    while (gameRunning && sceneManager.HasScene())
    {
        const DWORD currTime = timeGetTime();
        float deltaTime = static_cast<float>(currTime - lastFrameTime) / 1000.0f;
        if (deltaTime > FRAME_TIME * 2)
        {
            deltaTime = FRAME_TIME * 2;
        }

        if (deltaTime >= FRAME_TIME)
        {
            lastFrameTime = currTime;
            GetKeyAll();

            sceneManager.Update(deltaTime);

            if (ChkKeyEdge(PK_ESC))
            {
                gameRunning = false;
            }

            if (!sceneManager.GetScene()->IsSceneRunning())
            {
                gameRunning = false;
            }
        }
        else
        {
            Sleep(1);
        }
    }

    EndConioEx();

    return 0;
}
