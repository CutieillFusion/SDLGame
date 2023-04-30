#pragma once
#include "SDL.h"
#include <iostream>

class Vector2D
{
public:
	Vector2D();
	Vector2D(float x, float y);
	Vector2D(const Vector2D& vector);

	float Dot(const Vector2D& v);
	float Cross(const Vector2D& v);
	Vector2D TripleCross(Vector2D& v1, Vector2D& v2);

	float Distance(const Vector2D& v);
	float Magnitude();
	Vector2D Normalize();

	static float Distance(const Vector2D& v1, const Vector2D& v2);
	static float Magnitude(const Vector2D& v);
	static Vector2D Normalize(const Vector2D& v);

	static float Dot(const Vector2D& v1, const Vector2D& v2);

	static Vector2D Lerp(const Vector2D& start, const Vector2D& end, float delta);

	Vector2D operator *(float num);
	Vector2D operator +(Vector2D vec);
	Vector2D operator -(Vector2D vec);
	Vector2D operator -();

	void operator +=(Vector2D vec);
	void operator -=(Vector2D vec);
	void operator *=(float num);

	SDL_Point* ToSDLPoint();

	~Vector2D();

public:
	float x, y;
};

class Vector3D
{
public:
	Vector3D();
	Vector3D(float x, float y, float z);
	Vector3D(const Vector3D& vector);

	float Magnitude();
	Vector3D Normalize();

	//Two Versions because GJK Threw errors and this was a fix probably not needed 
	float Dot(const Vector3D& v);
	float Dot(Vector3D& v);
	Vector3D Cross(const Vector3D& v1);

	static float Distance(const Vector3D& v1, const Vector3D& v2);

	static float Dot(const Vector3D& v1, const Vector3D& v2);
	static Vector3D Cross(const Vector3D& v1, const Vector3D& v2);

	static Vector3D Lerp(const Vector3D& start, const Vector3D& end, float delta);

	//2D Perpendicular just a GJK Workaround
	Vector3D Perpendicular();

	Vector3D operator *(float num);
	Vector3D operator /(float num);
	Vector3D operator +(Vector3D vec);
	Vector3D operator -(Vector3D vec);

	Vector3D operator -();

	void operator +=(Vector3D vec);
	void operator +=(Vector2D vec);
	void operator -=(Vector3D vec);
	void operator -=(Vector2D vec);
	void operator *=(float num);

	~Vector3D();
public:
	float x, y, z;
};