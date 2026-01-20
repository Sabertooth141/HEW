//
// Created by saber on 1/14/2026.
//

#include "Entity.h"

class Camera;

Entity::Entity(const EntityConfig& config) : Object(config),
                                             velX(config.velX), velY(config.velY),
                                             gravity(config.gravity),
                                             maxFallSpeed(config.maxFallSpeed),
                                             currHp(config.currHp), maxHp(config.maxHp), isFacingRight(config.isFacingRight),
                                             moveSpeed(config.moveSpeed)

{
}

void Entity::Start(const float x, const float y)
{
    Object::Start(x, y);
}

void Entity::Update(const float deltaTime, const Tilemap& tileMap)
{
    Object::Update(deltaTime, tileMap);
    ApplyPhysics(deltaTime);
    isGrounded = CheckGrounded(tileMap);
}

void Entity::Draw(const Camera& cam)
{
    Object::Draw(cam);
}

void Entity::HandleMovement()
{
}

void Entity::TakDamage(const float inDamage)
{
    currHp -= inDamage;
    if (currHp <= 0)
    {
        currHp = 0;
        Die();
    }
}

void Entity::Die()
{
}

void Entity::ApplyPhysics(const float deltaTime)
{
    if (!isGrounded)
    {
        velY += gravity * deltaTime;

        if (velY > maxFallSpeed)
        {
            velY = maxFallSpeed;
        }
    }
}

bool Entity::CheckCollisionX(const Tilemap& tilemap, const float newX)
{
    const float checkX = velX > 0 ? newX + width - 1 : newX;

    const float topY = y + 1;
    const float bottomY = y + height - 2;

    return tilemap.IsSolidAt(checkX, bottomY) || tilemap.IsSolidAt(checkX, topY);
}

bool Entity::CheckCollisionY(const Tilemap& tilemap, const float newY)
{
    const float checkY = velY > 0 ? newY + height - 1 : newY;

    const float leftX = x + 1;
    const float rightX = x + width - 2;

    if (velY > 0)
    {
        const float currBottomY = y + height - 2;
        const float newBottomY = newY + height - 2;

        const int currTileY = tilemap.WorldToTileY(currBottomY);
        const int newTileY = tilemap.WorldToTileY(newBottomY);

        if (currTileY < newTileY)
        {
            if (tilemap.IsPlatformAt(leftX, newBottomY) || tilemap.IsPlatformAt(rightX, currBottomY))
            {
                return true;
            }
        }
    }

    return tilemap.IsSolidAt(leftX, checkY) || tilemap.IsSolidAt(rightX, checkY);
}

bool Entity::CheckGrounded(const Tilemap& tilemap)
{
    const float checkY = y + height;

    const float leftX = x + 1;
    const float rightX = x + width - 2;

    return tilemap.IsSolidAt(leftX, checkY) || tilemap.IsSolidAt(rightX, checkY)
        || tilemap.IsPlatformAt(leftX, checkY) || tilemap.IsPlatformAt(rightX, checkY);
}
