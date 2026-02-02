//
// Created by saber on 1/28/2026.
//

#ifndef HEW_PLAYERSTATES_H
#define HEW_PLAYERSTATES_H
#include <type_traits>

enum class PlayerNormalState : uint8_t
{
    IDLE = 0,
    MOVE,
    JUMP,
    FALL,
    DIE,
    DEFAULT
};

enum class PlayerCombatState : uint8_t
{
    ATTK0 = 0,
    ATTK1,
    ATTK2,
    ATTK_RECOVERY,
    DEFAULT
};

template <typename T>
concept IsAllowedType = std::is_same_v<T, PlayerNormalState> || std::is_same_v<T, PlayerCombatState>;

template <IsAllowedType T>
struct PlayerStateMachine
{
    T prevState;
    T currState;
    float stateTimer = 0.0f;

    explicit PlayerStateMachine(T defaultState) : prevState(defaultState), currState(defaultState)
    {
    }

    void ChangeState(const T newState)
    {
        if (newState == GetCurrState())
        {
            return;
        }

        prevState = currState;
        currState = newState;
        stateTimer = 0.0f;
    }

    void Update(const float deltaTime)
    {
        stateTimer += deltaTime;
    }

    [[nodiscard]] T GetCurrState() const { return currState; }
    [[nodiscard]] T GetPrevState() const { return prevState; }
    [[nodiscard]] float GetStateTimer() const { return stateTimer; }
};

#endif //HEW_PLAYERSTATES_H
