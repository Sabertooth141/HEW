//
// Created by saber on 1/21/2026.
//

#ifndef HEW_TIMEMANAGER_H
#define HEW_TIMEMANAGER_H
#include <vector>

#include "../Config/Structs.h"

class TimeManager
{
public:
    TimeManager();

    static TimeManager& Instance()
    {
        static TimeManager instance;
        return instance;
    }

    void Initialize(const TimeManagerConfig& config);
    void Update(float deltaTime);
    [[nodiscard]] float GetWorldDeltaTime(float realDeltaTime) const;

    // time stop
    void ActivateTimeStop(float duration);
    void DeactivateTimeStop();
    [[nodiscard]] bool IsTimeStopped() const { return isTimeStopped; }
    [[nodiscard]] float GetTimeStopDurationTimer() const { return timeStopDurationTimer; }

    // time rewind
    void RecordPlayerSnapshot(const PlayerSnapshot& snapshot);
    [[nodiscard]] bool GetPlayerSnapshot(PlayerSnapshot& outputSnapshot);
    [[nodiscard]] PlayerSnapshot GetPlayerSnapshot() const;
    [[nodiscard]] bool GetSnapshotAt(int frameBack, PlayerSnapshot& outSnapshot) const;
    bool StartRewind();
    bool UpdateRewind(float deltaTime, PlayerSnapshot& outSnapshot);
    [[nodiscard]] bool IsRewinding() const { return isRewinding; };

    // hit stop
    void TriggerHitStop(float duration);
    [[nodiscard]] bool IsHitStopped() const { return hitStopActive; }

private:
    // time stop
    bool isTimeStopped = false;
    float timeStopDurationTimer = 0;
    float timeStopCooldownTimer = 0;
    float timeStopDuration{};

    float timeStopCooldownMax{};

    // time rewind
    std::vector<PlayerSnapshot> rewindBuffer;
    int rewindIndex = 0;
    int snapshotsCnt = 0;

    int bufferHead = 0;

    float rewindCooldownTimer = 0;
    float rewindCooldownMax{};

    float rewindMagnitude = 0;

    bool isRewinding = false;
    float rewindPlayBackSpeed = 1.0f;
    float rewindAccumulator = 0;
    int rewindReadHead = 0;
    int rewindFramesLeft = 0;

    // hit stop
    float hitStopTimer = 0;
    bool hitStopActive = false;
};


#endif //HEW_TIMEMANAGER_H
