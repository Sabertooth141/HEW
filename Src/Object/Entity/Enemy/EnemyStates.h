//
// Created by saber on 2/4/2026.
//

#ifndef HEW_ENEMYSTATES_H
#define HEW_ENEMYSTATES_H
#include <cstdint>

enum class EnemyState : uint8_t
{
    IDLE = 0,
    MOVE,
    ATTK,
    DEFAULT
};

struct EnemyStateMachine
{
    EnemyState currState;
    EnemyState prevState;
    float stateTimer = 0.0f;

    explicit EnemyStateMachine(const EnemyState defaultState) : currState(defaultState), prevState(defaultState)
    {
    }

    void Update(const float deltaTime)
    {
        stateTimer += deltaTime;
    }

    void ChangeState(const EnemyState nextState)
    {
        if (currState == nextState)
        {
            return;
        }

        currState = nextState;
    }

    EnemyState GetCurrentState() const { return currState; }
    EnemyState GetPreviousState() const { return prevState; }
};

#endif //HEW_ENEMYSTATES_H
