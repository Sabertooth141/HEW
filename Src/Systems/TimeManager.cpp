//
// Created by saber on 1/21/2026.
//

#include "TimeManager.h"

#include "EnemyManager.h"
#include "../Game/Scenes/GameScene.h"
#include "../Game/Game.h"

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
    isRewindAttk = false;
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

    if (isRewinding || isRewindAttk)
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
        return {};
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

    const int bufferSize = static_cast<int>(rewindBuffer.size());
    const int index = ((bufferHead - 1 - frameBack) % bufferSize + bufferSize) % bufferSize;
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
    isTimeStopped = true;
    rewindAccumulator = 0;
    rewindReadHead = 0;
    rewindMaxFrames = snapshotsCnt;

    FindAttackFrames();

    return true;
}

bool TimeManager::UpdateRewind(float deltaTime, PlayerSnapshot& outSnapshot)
{
    if (!isRewinding)
    {
        return false;
    }

    rewindAccumulator += rewindPlayBackSpeed;

    while (rewindAccumulator >= 1 && rewindReadHead < rewindMaxFrames - 1)
    {
        rewindReadHead += 1;
        rewindAccumulator -= 1;
    }

    return GetSnapshotAt(rewindReadHead, outSnapshot);
}

void TimeManager::EndRewind(bool& outDoAttack, RewindAttackFrame& outTarget)
{
    isRewinding = false;
    rewindCooldownTimer = rewindCooldownMax;

    bufferHead = 0;
    rewindIndex = 0;
    snapshotsCnt = 0;

    if (hasAttackFrame)
    {
        if (std::abs(rewindReadHead - bestAttackFrame.snapshotIndex) <= rewindAttackTolerance)
        {
            outDoAttack = true;
            outTarget = bestAttackFrame;
            isRewindAttk = true;
            return;
        }
    }

    isRewindAttk = false;
    isTimeStopped = false;
    outDoAttack = false;
}

void TimeManager::FindAttackFrames()
{
    hasAttackFrame = false;
    float bestDist = 9999.0f;

    std::vector<Enemy*> enemies = EnemyManager::Instance().GetActiveEnemies();

    for (int i = 0; i < rewindMaxFrames; i++)
    {
        PlayerSnapshot playerSnap;
        if (!GetSnapshotAt(i, playerSnap))
        {
            continue;
        }

        for (auto& enemy : enemies)
        {
            if (!enemy->IsAlive())
            {
                continue;
            }

            float distance = Transform::GetDistance(playerSnap.transform.center, enemy->transform.center);

            if (distance < rewindAttackRange && distance < bestDist)
            {
                bestDist = distance;
                bestAttackFrame.snapshotIndex = i;
                bestAttackFrame.distToEnemy = distance;
                bestAttackFrame.targetEnemy = enemy;
                hasAttackFrame = true;
            }
        }
    }
}

void TimeManager::TriggerHitStop(const float duration)
{
    hitStopTimer = duration;
    hitStopActive = true;
}
