#pragma once
#include <iostream>
#include "SDL.h"

class Vector2D
{
public:
	Vector2D() : x(0), y(0)
	{

	}

	Vector2D(float x, float y) :x(x), y(y)
	{
	}

	Vector2D(const Vector2D& vector) :x(vector.x), y(vector.y)
	{

	}

	float Dot(const Vector2D& v)
	{
		return (v.x * x) + (v.y * y);
	}

	float Cross(const Vector2D& v) 
	{
		return ((x * v.y) - (v.x * y));
	}

	Vector2D TripleCross(Vector2D& v1, Vector2D& v2)
	{
		float crossX = (v1.x * (this->Dot(v2)) - (v2.x * this->Dot(v1)));
		float crossY = (v1.y * (this->Dot(v2)) - (v2.y * this->Dot(v1)));
		return Vector2D(crossX, crossY);
	}

	float Distance(const Vector2D& v)
	{
		return std::sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
	}

	float Magnitude()
	{
		if (x == 0 && y == 0)
		{
			return 0.0f;
		}
		return std::sqrtf((x * x) + (y * y));
	}

	Vector2D Normalize()
	{
		float magnitude = Magnitude();
		if (magnitude != 0.0f)
		{
			return Vector2D(x / magnitude, y / magnitude);
		}
		else
		{
			return Vector2D();
		}
	}

	static float Distance(const Vector2D& v1, const Vector2D& v2)
	{
		return std::sqrtf((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
	}

	static float Dot(const Vector2D& v1, const Vector2D& v2)
	{
		return (v1.x * v2.x) + (v1.y * v2.y);
	}

	static Vector2D Lerp(const Vector2D& start, const Vector2D& end, float delta)
	{
		Vector2D v;

		v.x = start.x * (1.0f - delta) + end.x * (delta);
		v.y = start.y * (1.0f - delta) + end.y * (delta);

		return v;
	}

	Vector2D operator *(float num)
	{
		return Vector2D(x * num, y * num);
	}

	Vector2D operator +(Vector2D vec)
	{
		return Vector2D(x + vec.x, y + vec.y);
	}

	Vector2D operator -(Vector2D vec)
	{
		return Vector2D(x - vec.x, y - vec.y);
	}

	Vector2D operator -()
	{
		return Vector2D(-x, -y);
	}

	void operator +=(Vector2D vec)
	{
		x += vec.x;
		y += vec.y;
	}

	void operator -=(Vector2D vec)
	{
		x -= vec.x;
		y -= vec.y;
	}

	void operator *=(float num)
	{
		x *= num;
		y *= num;
	}

	SDL_Point* ToSDLPoint() 
	{
		SDL_Point point = SDL_Point();
		point.x = x;
		point.y = y;
		return &point;
	}

	~Vector2D()
	{

	}

public:
	float x, y;
};

class Vector3D
{
public:
	Vector3D() : x(0), y(0), z(0)
	{

	}

	Vector3D(float x, float y, float z) : x(x), y(y), z(z)
	{
	}

	Vector3D(const Vector3D& vector) :x(vector.x), y(vector.y), z(vector.z)
	{

	}

	float Magnitude()
	{
		if (x == 0 && y == 0 && z == 0)
		{
			return 0.0f;
		}
		return std::sqrtf((x * x) + (y * y) + (z * z));
	}

	Vector3D Normalize()
	{
		float magnitude = Magnitude();
		if (magnitude > 0.0f)
		{
			return Vector3D(x / magnitude, y / magnitude, z / magnitude);
		}
		else
		{
			return Vector3D();
		}
	}

	float Dot(const Vector3D& v)
	{
		return (x * v.x) + (y * v.y) + (z * v.z);
	}

	float Dot(Vector3D& v)
	{
		return (x * v.x) + (y * v.y) + (z * v.z);
	}

	Vector3D Cross(const Vector3D& v1)
	{
		Vector3D v;

		v.x = (y * v1.z) - (z * v1.y);
		v.y = (z * v1.x) - (x * v1.z);
		v.z = (x * v1.y) - (y * v1.x);

		return v;
	}

	static float Distance(const Vector3D& v1, const Vector3D& v2)
	{
		return std::sqrtf((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y) + (v1.z - v2.z) * (v1.z - v2.z));
	}

	static float Dot(const Vector3D& v1, const Vector3D& v2)
	{
		return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	}

	static Vector3D Cross(const Vector3D& v1, const Vector3D& v2)
	{
		Vector3D v;

		v.x = (v1.y * v2.z) - (v1.z * v2.y);
		v.y = (v1.z * v2.x) - (v1.x * v2.z);
		v.z = (v1.x * v2.y) - (v1.y * v2.x);

		return v;
	}

	static Vector3D Lerp(const Vector3D& start, const Vector3D& end, float delta)
	{
		Vector3D v;

		v.x = start.x * (1.0f - delta) + end.x * (delta);
		v.y = start.y * (1.0f - delta) + end.y * (delta);
		v.z = start.z * (1.0f - delta) + end.z * (delta);

		return v;
	}

	Vector3D Perpendicular() 
	{
		return Vector3D(y, -x, z);
	}

	Vector3D operator *(float num)
	{
		return Vector3D(x * num, y * num, z * num);
	}

	Vector3D operator /(float num)
	{
		return Vector3D(num / x, num / y, num / z);
	}

	Vector3D operator +(Vector3D vec)
	{
		return Vector3D(x + vec.x, y + vec.y, z + vec.z);
	}

	Vector3D operator -(Vector3D vec)
	{
		return Vector3D(x - vec.x, y - vec.y, z - vec.z);
	}
	Vector3D operator -()
	{
		return Vector3D(-x, -y, -z);
	}

	~Vector3D()
	{

	}
public:
	float x, y, z;
};