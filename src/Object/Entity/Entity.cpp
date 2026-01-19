//
// Created by saber on 1/14/2026.
//

#include "Entity.h"

class Camera;

Entity::Entity() : currHp(0),
                   maxHp(0),
                   isGrounded(false),
                   isFacingRight(false)
{
}

void Entity::Start(float x, float y)
{
}

void Entity::Update(float deltaTime, const Tilemap& tileMap)
{
}

void Entity::Draw(const Camera& cam)
{
}

void Entity::TakDamage()
{
}

void Entity::Die()
{
}

void Entity::ApplyPhysics(float deltaTime)
{
}

bool Entity::CheckCollisionX(const Tilemap& tilemap, float newX)
{
}

bool Entity::CheckCollisionY(const Tilemap& tilemap, float newY)
{
}

bool Entity::CheckGrounded(const Tilemap& tilemap)
{
}
