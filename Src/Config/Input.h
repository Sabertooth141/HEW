//
// Created by saber on 2/19/2026.
//

#ifndef HEW_INPUT_H
#define HEW_INPUT_H
#include "../Lib/conioex.h"

struct GamepadState
{
    float leftStickX = 0;
    float leftStickY = 0;
    float rightStickX = 0;
    float rightStickY = 0;
    float leftTrigger = 0;
    float rightTrigger = 0;
    WORD buttons = 0;
    WORD prevButtons = 0;
    bool connected = false;
};

inline GamepadState gamePad = {};

void UpdateGamepad(int id = 0);

struct GamepadBinding
{
    enum class Type
    {
        BUTTON,
        AXIS_POSITIVE,
        AXIS_NEGATIVE,
        TRIGGER,
        DEFAULT
    };

    Type type = Type::DEFAULT;
    WORD buttonMask = 0;
};

struct KeyBinding
{
    int primary = -1;
    int secondary = -1;
    GamepadBinding pad = {};

    [[nodiscard]] bool IsPressed() const
    {
        const bool kb = (primary != -1 && ChkKeyPress(primary)) || (secondary != -1 && ChkKeyPress(secondary));
        return kb || IsGamepadActive();
    }

    [[nodiscard]] bool IsEdge() const
    {
        const bool kb = (primary != -1 && ChkKeyEdge(primary)) || (secondary != -1 && ChkKeyEdge(secondary));
        return kb || IsGamepadEdge();
    }

private:
    [[nodiscard]] bool IsGamepadActive() const
    {
        if (!gamePad.connected)
        {
            return false;
        }

        switch (pad.type)
        {
        case GamepadBinding::Type::BUTTON:
            return (gamePad.buttons & pad.buttonMask) != 0;
        case GamepadBinding::Type::AXIS_POSITIVE:
            return gamePad.leftStickX > 0.3f;
        case GamepadBinding::Type::AXIS_NEGATIVE:
            return gamePad.leftStickX < -0.3f;
        case GamepadBinding::Type::TRIGGER:
            return gamePad.rightTrigger > 0.3f;
        default:
            return false;
        }
    }

    [[nodiscard]] bool IsGamepadEdge() const
    {
        if (!gamePad.connected)
        {
            return false;
        }

        if (pad.type == GamepadBinding::Type::BUTTON)
        {
            bool pressedNow = (gamePad.buttons & pad.buttonMask) != 0;
            bool pressedPrev = (gamePad.prevButtons & pad.buttonMask) != 0;
            return pressedNow && !pressedPrev;
        }

        return IsGamepadActive();
    }
};

struct GlobalInputConfig
{
    // menu select
    KeyBinding quitGame = {PK_ESC, PK_DEL, {GamepadBinding::Type::BUTTON, XINPUT_GAMEPAD_BACK}};
    KeyBinding selectLeft = {PK_A, PK_LEFT, {GamepadBinding::Type::BUTTON, XINPUT_GAMEPAD_DPAD_LEFT}};
    KeyBinding selectRight = {PK_D, PK_RIGHT, {GamepadBinding::Type::BUTTON, XINPUT_GAMEPAD_DPAD_RIGHT}};
    KeyBinding selectUp = {PK_W, PK_UP, {GamepadBinding::Type::BUTTON, XINPUT_GAMEPAD_DPAD_UP}};
    KeyBinding selectDown = {PK_S, PK_DOWN, {GamepadBinding::Type::BUTTON, XINPUT_GAMEPAD_DPAD_DOWN}};
    KeyBinding confirm = {PK_SP, PK_ENTER, {GamepadBinding::Type::BUTTON, XINPUT_GAMEPAD_A}};
    KeyBinding back = {PK_BS, -1, {GamepadBinding::Type::BUTTON, XINPUT_GAMEPAD_B}};
};

struct InputConfig
{
    // common
    KeyBinding moveLeft = {PK_LEFT, PK_A, {GamepadBinding::Type::AXIS_NEGATIVE}};
    KeyBinding moveRight = {PK_RIGHT, PK_D, {GamepadBinding::Type::AXIS_POSITIVE}};
    KeyBinding jump = {PK_SP, PK_W, {GamepadBinding::Type::BUTTON, XINPUT_GAMEPAD_A}};
    KeyBinding attack = {PK_J, -1, {GamepadBinding::Type::BUTTON, XINPUT_GAMEPAD_X}};

    // time skills
    KeyBinding timeStop = {PK_K, -1, {GamepadBinding::Type::BUTTON, XINPUT_GAMEPAD_RIGHT_SHOULDER}};
    KeyBinding timeRewind = {PK_SHIFT, -1, {GamepadBinding::Type::BUTTON, XINPUT_GAMEPAD_LEFT_SHOULDER}};
};


#endif //HEW_INPUT_H
