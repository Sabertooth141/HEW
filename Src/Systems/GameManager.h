//
// Created by saber on 2/13/2026.
//

#ifndef HEW_GAMEMANAGER_H
#define HEW_GAMEMANAGER_H
#include "../Object/Entity/Player/PlayerController.h"
#include "../World/Camera.h"

class PlayerController;

struct GameManager
{
    static GameManager& Instance()
    {
        static GameManager instance;
        return instance;
    }

    void RegisterPlayer(PlayerController& player)
    {
        activePlayer = &player;
    }

    void UnregisterPlayer()
    {
        activePlayer = nullptr;
    }

    PlayerController* GetActivePlayer() const
    {
        return activePlayer;
    }

    void SetIsVictory(const bool inStatus)
    {
        isVictory = inStatus;
    }

    bool GetIsVictory() const
    {
        return isVictory;
    }

private:
    PlayerController* activePlayer = nullptr;
    bool isVictory = false;

};

#endif //HEW_GAMEMANAGER_H