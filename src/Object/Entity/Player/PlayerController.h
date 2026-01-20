//
// Created by saber on 1/14/2026.
//

#ifndef HEW_PLAYERCONTROLLER_H
#define HEW_PLAYERCONTROLLER_H
#include "../Entity.h"
#include "../../../World/Camera.h"
#include "../../../World/Tilemap.h"


struct PlayerConfig : EntityConfig
{
    float sprintSpeed;
};

class Camera;

class PlayerController : public Entity
{
    // functions
public:
    explicit PlayerController(const PlayerConfig& config);

    void Start(float x, float y) override;
    void Update(float deltaTime, const Tilemap& tileMap) override;
    void Draw(const Camera& cam) override;
    void Die() override;


    // vars
private:
    float sprintSpeed;
};


#endif //HEW_PLAYERCONTROLLER_H
