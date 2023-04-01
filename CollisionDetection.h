#pragma once
#include "Globals.h"
#include "ColliderComponent.h"
#include "UIColliderComponent.h"
#include "Vector.h"

struct CollisionPoint
{
	Vector3D normal;
	bool colliding;
};

//Abstract Collider Component maybe
class CollisionDetection
{
public:
	//Find Farthest Point Function
	Vector3D FindFarthestPoint(ColliderComponent* collider, Vector3D direction);
	Vector3D FindFarthestPoint(UIColliderComponent* collider, Vector3D direction);

	//Minkowski Sum Function
	Vector3D Support(ColliderComponent* colliderA, ColliderComponent* colliderB, Vector3D direction);
	Vector3D Support(UIColliderComponent* colliderA, SDL_Rect* mousePos, Vector3D direction);

	//Main GJK Algorithm Function
	CollisionPoint GJK(ColliderComponent* colliderA, ColliderComponent* colliderB);
	bool GJK(UIColliderComponent* colliderA, SDL_Rect* mousePos);

	//Main EPA Algorithm Function
	Vector3D EPA(std::vector<Vector3D> polytope, ColliderComponent* colliderA, ColliderComponent* colliderB);
};


