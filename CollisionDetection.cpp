#include "CollisionDetection.h"

Vector3D CollisionDetection::FindFarthestPoint(ColliderComponent* collider, Vector3D direction)
{
	Vector3D maxPoint;
	float maxDistance = -FLT_MAX;

	for (Vector3D vertex : collider->movingMesh)
	{
		float distance = vertex.Dot(direction);
		if (distance > maxDistance)
		{
			maxDistance = distance;
			maxPoint = vertex;
		}
	}

	return maxPoint;
}

Vector3D CollisionDetection::Support(ColliderComponent* colliderA, ColliderComponent* colliderB, Vector3D direction)
{
	Vector3D a = FindFarthestPoint(colliderA, direction);
	Vector3D b = FindFarthestPoint(colliderB, -direction);

	return a - b;
}

CollisionPoint CollisionDetection::GJK(ColliderComponent* colliderA, ColliderComponent* colliderB)
{
	CollisionPoint collisionPoint = { Vector3D(), false };

	int index = 0;
	Vector3D a, b, c, ao, ab, ac, abperp, acperp;
	std::vector<Vector3D> simplex = { Vector3D(), Vector3D(), Vector3D() };
	Vector3D direction = Support(colliderA, colliderB, Vector3D(1, 0, 0));

	a = simplex[0] = Support(colliderA, colliderB, direction);

	if (a.Dot(direction) <= 0)
	{
		//No Collision
		return collisionPoint;
	}

	direction = -a;

	while (true)
	{
		a = simplex[++index] = Support(colliderA, colliderB, direction);

		if (a.Dot(direction) <= 0)
		{
			//No Collision
			return collisionPoint;
		}

		ao = -a;

		if (index < 2)
		{
			b = simplex[0];
			ab = b - a;
			direction = ab.Cross(ao).Cross(ab);
			if (direction.Magnitude() == 0)
			{
				direction = ab.Perpendicular();
			}
			continue;
		}

		b = simplex[1];
		c = simplex[0];
		ab = b - a;
		ac = c - a;

		acperp = ab.Cross(ac).Cross(ac);

		if (acperp.Dot(ao) >= 0)
		{
			//New direction is the Normal to Ac towards the Origin
			direction = acperp;
		}
		else
		{
			abperp = ac.Cross(ab).Cross(ab);

			if (abperp.Dot(ao) < 0)
			{
				//Collision
				collisionPoint.colliding = true;
				collisionPoint.normal = EPA(simplex, colliderA, colliderB);
				return collisionPoint;
			}

			//Swap First Element (Point C)
			simplex[0] = simplex[1];

			direction = abperp;
		}

		//Swap Element in the Middle (Point B)
		simplex[1] = simplex[2];
		--index;
	}

	return collisionPoint;
}

Vector3D CollisionDetection::EPA(std::vector<Vector3D> polytope, ColliderComponent* colliderA, ColliderComponent* colliderB)
{
	int minIndex = 0;
	float minDistance = INFINITY;
	Vector3D minNormal;

	while (minDistance == INFINITY)
	{
		for (int i = 0; i < polytope.size(); i++)
		{
			int j = (i + 1) % polytope.size();

			Vector3D vertexI = polytope[i];
			Vector3D vertexJ = polytope[j];

			Vector3D ij = vertexJ - vertexI;

			//Get Normal
			Vector3D normal = Vector3D(ij.y, -ij.x, 0).Normalize();
			float distance = normal.Dot(vertexI);

			//Checks Winding Order
			if (distance < 0)
			{
				//Flips Normal
				distance *= -1;
				normal = normal * -1;
			}

			//Finds Min Distance
			if (distance < minDistance)
			{
				minDistance = distance;
				minNormal = normal;
				minIndex = j;
			}
		}

		Vector3D support = Support(colliderA, colliderB, minNormal);
		float sDistance = minNormal.Dot(support);

		if (abs(sDistance - minDistance) > 0.001f)
		{
			minDistance = INFINITY;
			polytope.push_back(support);
		}
	}
	//Returns plus a small distance to stop repeat Collisions
	return minNormal * (minDistance + 0.001f);
}
