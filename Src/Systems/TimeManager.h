//
// Created by saber on 1/21/2026.
//

#ifndef HEW_TIMEMANAGER_H
#define HEW_TIMEMANAGER_H

struct TimeMangerConfig
{
    float timeStopDuration;
    float timeStopCooldownMax;
};

class TimeManager
{
public:
    TimeManager();

    static TimeManager& Instance()
    {
        static TimeManager instance;
        return instance;
    }

    void ActivateTimeStop(float duration);
    void DeactivateTimeStop();
    [[nodiscard]] bool IsTimeStopped() const { return isTimeStopped; }
    [[nodiscard]] float GetTimeStopDurationTimer() const { return timeStopDurationTimer; }

    void Update(float deltaTime);
    [[nodiscard]] float GetWorldDeltaTime(float realDeltaTime) const;

private:
    bool isTimeStopped = false;
    float timeStopDurationTimer = 0;
    float timeStopCooldownTimer = 0;

    float timeStopDuration;
    float timeStopCooldownMax;
};


#endif //HEW_TIMEMANAGER_H
