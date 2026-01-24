//
// Created by saber on 1/14/2026.
//

#include "Entity.h"

#include "../../Lib/Shape.h"
#include "../../Systems/EnemyManager.h"

class Camera;

Entity::Entity()
= default;

void Entity::Initialize(const EntityConfig& config)
{
    Object::Initialize(config);

    velX = config.velX;
    velY = config.velY;
    gravity = config.gravity;
    maxFallSpeed = config.maxFallSpeed;
    currHp = config.currHp;
    maxHp = config.maxHp;
    isFacingRight = config.isFacingRight;

    if (currHp == 0)
    {
        currHp = maxHp;
    }

    damage = config.damage;
    attackCooldown = config.attackCooldown;
    attackCooldownTimer = 0;
    attackOffsetX = config.attackOffsetX;
    attackOffsetY = config.attackOffsetY;
    attackWidth = config.attackWidth;
    attackHeight = config.attackHeight;
    attackDuration = config.attackDuration;
}

void Entity::Start()
{
    Object::Start();
}

void Entity::Update(const float deltaTime, const Tilemap& tileMap)
{
    Object::Update(deltaTime, tileMap);
    ApplyPhysics(deltaTime);
    isGrounded = CheckGrounded(tileMap);

    HandleMovement(deltaTime, tileMap);

    if (attackCooldownTimer > 0)
    {
        attackCooldownTimer -= deltaTime;
    }

    attackHitbox.Update(deltaTime, x, y);

    if (attackHitbox.isActive)
    {
        for (const auto enemy : EnemyManager::Instance().GetActiveEnemies())
        {
            if (attackHitbox.CheckOverlap(enemy->GetX(), enemy->GetY(), enemy->GetWidth(), enemy->GetHeight()))
            {
                enemy->TakeDamage(damage);

                attackHitbox.isActive = false;
                break;
            }
        }
    }
}

void Entity::Draw(const Camera& cam)
{
    Object::Draw(cam);
}

void Entity::HandleMovement(const float deltaTime, const Tilemap& tileMap)
{
    float newX = x + velX * deltaTime;

    if (!CheckCollisionX(tileMap, newX))
    {
        x = newX;
    }
    else
    {
        velX = 0;
    }

    float newY = y + velY * deltaTime;

    if (!CheckCollisionY(tileMap, newY))
    {
        y = newY;
    }
    else
    {
        if (velY > 0)
        {
            const int tileY = tileMap.WorldToTileY(newY + height);
            y = tileMap.TileToWorldY(tileY) - height;
            isGrounded = true;
        }
        velY = 0;
    }

    isGrounded = CheckGrounded(tileMap);
}

void Entity::TakeDamage(const float inDamage)
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

void Entity::Attack()
{
    if (attackCooldownTimer > 0)
    {
        return;
    }

    float hitboxOffsetX;
    if (isFacingRight)
    {
        hitboxOffsetX = width + attackOffsetX;
    }
    else
    {
        hitboxOffsetX = -attackOffsetX - attackWidth;
    }
    const float hitboxOffsetY = attackOffsetY;

    attackHitbox.Activate(x, y, hitboxOffsetX, hitboxOffsetY, attackWidth, attackHeight, attackDuration);
    attackCooldownTimer = attackCooldown;
}
