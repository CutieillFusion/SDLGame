#pragma once
#include "SpriteComponent.h"
#include "ECS.h"
#include <vector>
#include <map>

struct AnimationState
{
	int index;
	int length;
};

class AnimationComponent : public Component
{
public:
	AnimationComponent() = default;
	AnimationComponent(std::vector<AnimationState> animation);

	void Initialize() override;
	void Update() override;
	void Render() override;

private:
	int frames = 0;
	SpriteComponent* sprites;
	int animationIndex;
	std::vector<AnimationState> animation;
};