//
// Created by saber on 1/21/2026.
//

#include "TimeManager.h"

#include "../Core/Game.h"
#include "../Lib/conioex_custom.h"

TimeManager::TimeManager()
= default;

void TimeManager::Initialize(const TimeManagerConfig& config)
{
    timeStopCooldownMax = config.timeStopCooldownMax;

    rewindCooldownMax = config.rewindCooldownMax;
    rewindMagnitude = config.rewindMagnitude;
    rewindBuffer.resize(static_cast<int>(rewindMagnitude * TARGET_FPS));
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

    if (rewindCooldownTimer > 0)
    {
        rewindCooldownTimer -= deltaTime;
    }
}

float TimeManager::GetWorldDeltaTime(const float realDeltaTime) const
{
    return isTimeStopped ? realDeltaTime * 0.01f : realDeltaTime;
}

void TimeManager::RecordPlayerSnapshot(const PlayerSnapshot& snapshot)
{
    DebugPrintf("%d", rewindIndex);
    rewindBuffer[rewindIndex] = snapshot;
    rewindIndex = (rewindIndex + 1) % static_cast<int>(rewindBuffer.size());

    if (snapshotsCnt < rewindBuffer.size())
    {
        snapshotsCnt += 1;
    }
}

// THIS VERSION CHECKS IF TIME REWIND IS STILL IN COOLDOWN
bool TimeManager::GetPlayerSnapshot(PlayerSnapshot& outputSnapshot)
{
    if (snapshotsCnt == 0)
    {
        return false;
    }

    if (rewindCooldownTimer > 0)
    {
        return false;
    }

    rewindCooldownTimer = rewindCooldownMax;
    const int oldestIndex = snapshotsCnt < rewindBuffer.size() ? 0 : rewindIndex;
    outputSnapshot = rewindBuffer[oldestIndex];

    return true;
}

// THIS VERSION RETURNS SNAPSHOT NO MATTER THE COOLDOWN
PlayerSnapshot TimeManager::GetPlayerSnapshot() const
{
    if (snapshotsCnt == 0)
    {
        return{};
    }

    const int oldestIndex = snapshotsCnt < rewindBuffer.size() ? 0 : rewindIndex;
    return rewindBuffer[oldestIndex];
}
