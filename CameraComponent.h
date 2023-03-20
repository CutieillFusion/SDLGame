#pragma once
#include "ECS.h"
#include "Vector.h"

class CameraComponent : public Component
{
public:
	CameraComponent();

	void Initialize() override;
	void Update() override;
	void Render() override;

	static CameraComponent* camera;
	Vector3D cameraOffset;
};