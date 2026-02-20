//
// Created by saber on 2/19/2026.
//

#include "Input.h"

void UpdateGamepad(const int id)
{
    gamePad.prevButtons = gamePad.buttons;

    const int btns = InputJoystickX(id, PJX_BTNS);
    if (btns == -1)
    {
        gamePad.connected = false;
        gamePad.buttons = 0;
        return;
    }

    gamePad.connected = true;
    gamePad.buttons = static_cast<WORD>(btns);

    auto deadzone = [](const int raw, const int dead) -> float
    {
        if (abs(raw) < dead)
        {
            return 0;
        }

        float sign;
        if (raw > 0)
        {
            sign = 1;
        }
        else
        {
            sign = -1;
        }
        return sign * static_cast<float>(abs(raw) - dead) / (32767 - dead);
    };

    gamePad.leftStickX = deadzone(InputJoystickX(id, PJX_LXPOS), XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    gamePad.leftStickY = deadzone(InputJoystickX(id, PJX_LYPOS), XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    gamePad.rightStickX = deadzone(InputJoystickX(id, PJX_RXPOS), XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
    gamePad.rightStickY = deadzone(InputJoystickX(id, PJX_RYPOS), XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
    gamePad.leftTrigger = static_cast<float>(InputJoystickX(id, PJX_LTRG)) / 255.0f;
    gamePad.rightTrigger =  static_cast<float>(InputJoystickX(id, PJX_RTRG)) / 255.0f;
}

#include "Input.h"