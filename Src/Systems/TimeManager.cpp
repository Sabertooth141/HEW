//
// Created by saber on 1/21/2026.
//

#include "TimeManager.h"

TimeManager::TimeManager()
{
    timeStopDuration = 0;
    timeStopCooldownMax = 0;
}

void TimeManager::ActivateTimeStop(const float duration)
{
    if (timeStopCooldownTimer > 0)
    {
        return;
    }

    isTimeStopped = true;
    timeStopDurationTimer = duration;
}

void TimeManager::DeactivateTimeStop()
{
    isTimeStopped = false;
    timeStopDurationTimer = 0;
    timeStopCooldownTimer = timeStopCooldownMax;
}

void TimeManager::Update(const float deltaTime)
{
    if (timeStopCooldownTimer > 0)
    {
        timeStopCooldownTimer -= deltaTime;
    }

    if (isTimeStopped)
    {
        timeStopDurationTimer -= deltaTime;
        if (timeStopDurationTimer <= 0)
        {
            DeactivateTimeStop();
        }
    }
}

float TimeManager::GetWorldDeltaTime(const float realDeltaTime) const
{
    return isTimeStopped ? 0 : realDeltaTime;
}
