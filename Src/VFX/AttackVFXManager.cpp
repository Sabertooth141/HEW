//
// Created by A.Aidus on 2/8/2026.
//

#include "AttackVFXManager.h"
#include "../Config/Structs.h"

void AttackVFXManager::Update(const float deltaTime)
{
    if (playingEffect.empty())
    {
        return;
    }

    for (int i = 0; i < playingEffect.size(); i++)
    {
        if (playingEffect[i].animator == nullptr)
        {
            continue;
        }

        if (!playingEffect[i].animator->IsPlaying())
        {
            playingEffect.erase(playingEffect.begin() + i);
            continue;
        }

        playingEffect[i].animator->Update(deltaTime);
    }
}

void AttackVFXManager::Draw(const Camera& camera)
{
    if (playingEffect.empty())
    {
        return;
    }

    for (auto& effect : playingEffect)
    {
        if (effect.animator == nullptr)
        {
            continue;
        }

        if (effect.attached)
        {
            effect.transform.center.x = effect.followTargetTransform->center.x + effect.offSet.x;
            effect.transform.center.y = effect.followTargetTransform->center.y + effect.offSet.y;
            effect.transform.CalculateTopLeftPosition();

            effect.animator->Draw(camera, effect.transform.topLeft.x, effect.transform.topLeft.y,
                                  effect.flipHorizontal);
        }
        else
        {
            effect.animator->Draw(camera, effect.transform.topLeft.x, effect.transform.topLeft.y,
                                  effect.flipHorizontal);
        }
    }
}

void AttackVFXManager::PlayAttackVFX(const Transform* playTransform,
                                     const Vector2 offset, const PlayerCombatState playerState,
                                     const bool flipHorizontal, const bool attached)
{
    Animator* animatorToPlay = vfxAnimators.GetAnimator(playerState);
    if (animatorToPlay == nullptr)
    {
        return;
    }

    Transform effectTransform;;
    effectTransform.center.x = playTransform->center.x + offset.x;
    effectTransform.center.y = playTransform->center.y + offset.y;
    effectTransform.size.x = animatorToPlay->GetSpriteSheet()->frames[0].w;
    effectTransform.size.y = animatorToPlay->GetSpriteSheet()->frames[0].h;
    effectTransform.CalculateTopLeftPosition();

    SplashTrailEffect effectToAdd;
    effectToAdd.offSet = offset;
    effectToAdd.animator = animatorToPlay;
    effectToAdd.transform = effectTransform;
    effectToAdd.followTargetTransform = playTransform;
    effectToAdd.flipHorizontal = flipHorizontal;
    effectToAdd.isLoop = false;
    effectToAdd.attached = attached;

    playingEffect.push_back(effectToAdd);
    animatorToPlay->Play(effectToAdd.isLoop);
}

void AttackVFXManager::PlayAttackVFX(const Transform* playTransform, const Vector2 offset, const EnemyState enemyState, const bool flipHorizontal, const bool attached)
{
    Animator* animatorToPlay = vfxAnimators.GetAnimator(enemyState);
    if (animatorToPlay == nullptr)
    {
        return;
    }

    Transform effectTransform;;
    effectTransform.center.x = playTransform->center.x + offset.x;
    effectTransform.center.y = playTransform->center.y + offset.y;
    effectTransform.size.x = animatorToPlay->GetSpriteSheet()->frames[0].w;
    effectTransform.size.y = animatorToPlay->GetSpriteSheet()->frames[0].h;
    effectTransform.CalculateTopLeftPosition();

    SplashTrailEffect effectToAdd;
    effectToAdd.offSet = offset;
    effectToAdd.animator = animatorToPlay;
    effectToAdd.transform = effectTransform;
    effectToAdd.followTargetTransform = playTransform;
    effectToAdd.flipHorizontal = flipHorizontal;
    effectToAdd.isLoop = false;
    effectToAdd.attached = attached;

    playingEffect.push_back(effectToAdd);
    animatorToPlay->Play(effectToAdd.isLoop);
}

void AttackVFXManager::InitAnimation(const PlayerCombatAnimPaths& path)
{
    std::unique_ptr<Animator> animator = std::make_unique<Animator>();
    animator->LoadSpriteSheet(path.jsonPath.c_str(), path.bmpPath.c_str());
    animator->SetLoopStartFrame(path.startFrame);

    vfxAnimators.AddAnimator(path.animationState, std::move(animator));
}

void AttackVFXManager::InitAnimation(const EnemyAnimPaths& path)
{
    std::unique_ptr<Animator> animator = std::make_unique<Animator>();
    animator->LoadSpriteSheet(path.jsonPath.c_str(), path.bmpPath.c_str());
    animator->SetLoopStartFrame(path.startFrame);

    vfxAnimators.AddAnimator(path.animationState, std::move(animator));
}
