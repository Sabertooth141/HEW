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
    if (hitStopActive)
    {
        hitStopTimer -= deltaTime;
        if (hitStopTimer <= 0)
        {
            hitStopTimer = 0;
            hitStopActive = false;
        }
        return;
    }

    if (isRewinding)
    {
        return;
    }

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
    rewindBuffer[rewindIndex] = snapshot;
    rewindIndex = (rewindIndex + 1) % static_cast<int>(rewindBuffer.size());

    bufferHead += 1;
    bufferHead = bufferHead % static_cast<int>(rewindBuffer.size());

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

bool TimeManager::GetSnapshotAt(const int frameBack, PlayerSnapshot& outSnapshot) const
{
    if (frameBack >= snapshotsCnt || frameBack < 0)
    {
        return false;
    }

    const int index = (bufferHead - frameBack + snapshotsCnt) % snapshotsCnt;
    outSnapshot = rewindBuffer[index];
    return true;
}

bool TimeManager::StartRewind()
{
    if (rewindCooldownTimer > 0 || isRewinding)
    {
        return false;
    }

    if (snapshotsCnt == 0)
    {
        return false;
    }

    isRewinding = true;
    rewindAccumulator = 0;
    rewindReadHead = 0;
    rewindFramesLeft = snapshotsCnt;

    return true;
}

bool TimeManager::UpdateRewind(float deltaTime, PlayerSnapshot& outSnapshot)
{
    if (!isRewinding)
    {
        return false;
    }

    rewindAccumulator += rewindPlayBackSpeed;

    while (rewindAccumulator >= 1 && rewindFramesLeft > 0)
    {
        rewindReadHead += 1;
        rewindFramesLeft -= 1;
        rewindAccumulator -= 1;
    }

    if (!GetSnapshotAt(rewindReadHead, outSnapshot))
    {
        isRewinding = false;
        rewindCooldownTimer = rewindCooldownMax;
        return false;
    }

    if (rewindFramesLeft <= 0)
    {
        isRewinding = false;
        rewindCooldownTimer = rewindCooldownMax;
    }

    return true;
}

void TimeManager::TriggerHitStop(const float duration)
{
    hitStopTimer = duration;
    hitStopActive = true;
}
