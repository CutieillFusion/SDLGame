#include "AnimationComponent.h"

AnimationComponent::AnimationComponent(std::vector<AnimationState> animation)
{
	AnimationComponent::animation = animation;
}

void AnimationComponent::Initialize()
{
	sprites = &entity->getComponent<SpriteComponent>();
}

void AnimationComponent::Update()
{
	if (animationIndex >= animation.size()) 
	{
		animationIndex = 0;
	}

	if (frames++ >= animation[animationIndex].length)
	{
		sprites->SetIndex(animation[animationIndex++].index);
		frames = 0;
	}
}

void AnimationComponent::Render()
{
}