#pragma once
#include "Vector.h"
#include "ColliderComponents.h"

struct CollisionPoint
{
	Vector3D normal;
	bool colliding;
};

class CollisionDetection
{
public:
	//Find Farthest Point Function
	Vector3D FindFarthestPoint(ColliderComponent* collider, Vector3D direction);

	//Minkowski Sum Function
	Vector3D Support(ColliderComponent* colliderA, ColliderComponent* colliderB, Vector3D direction);

	//Main GJK Algorithm Function
	CollisionPoint GJK(ColliderComponent* colliderA, ColliderComponent* colliderB);

	//Main EPA Algorithm Function
	Vector3D EPA(std::vector<Vector3D> polytope, ColliderComponent* colliderA, ColliderComponent* colliderB);
};


