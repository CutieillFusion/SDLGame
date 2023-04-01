#include "Vector.h"

#pragma region VECTOR2D

Vector2D::Vector2D(): x(0), y(0)
{

}

Vector2D::Vector2D(float x, float y) : x(x), y(y)
{
}

Vector2D::Vector2D(const Vector2D& vector) : x(vector.x), y(vector.y)
{

}

float Vector2D::Dot(const Vector2D& v)
{
	return (v.x * x) + (v.y * y);
}

float Vector2D::Cross(const Vector2D& v) 
{
	return ((x * v.y) - (v.x * y));
}

Vector2D Vector2D::TripleCross(Vector2D& v1, Vector2D& v2)
{
	float crossX = (v1.x * (this->Dot(v2)) - (v2.x * this->Dot(v1)));
	float crossY = (v1.y * (this->Dot(v2)) - (v2.y * this->Dot(v1)));
	return Vector2D(crossX, crossY);
}

float Vector2D::Distance(const Vector2D& v)
{
	return std::sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
}

float Vector2D::Magnitude()
{
	if (x == 0 && y == 0)
	{
		return 0.0f;
	}
	return std::sqrtf((x * x) + (y * y));
}

Vector2D Vector2D::Normalize()
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

float Vector2D::Distance(const Vector2D& v1, const Vector2D& v2)
{
	return std::sqrtf((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}

float Vector2D::Magnitude(const Vector2D& v)
{
	if (v.x == 0 && v.y == 0)
	{
		return 0.0f;
	}
	return std::sqrtf((v.x * v.x) + (v.y * v.y));
}

Vector2D Vector2D::Normalize(const Vector2D& v)
{
	float magnitude = Magnitude(v);
	if (magnitude != 0.0f)
	{
		return Vector2D(v.x / magnitude, v.y / magnitude);
	}
	else
	{
		return Vector2D();
	}
}

float Vector2D::Dot(const Vector2D& v1, const Vector2D& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y);
}

Vector2D Vector2D::Lerp(const Vector2D& start, const Vector2D& end, float delta)
{
	Vector2D v;

	v.x = start.x * (1.0f - delta) + end.x * (delta);
	v.y = start.y * (1.0f - delta) + end.y * (delta);

	return v;
}

Vector2D Vector2D::operator*(float num)
{
	return Vector2D(x * num, y * num);
}

Vector2D Vector2D::operator+(Vector2D vec)
{
	return Vector2D(x + vec.x, y + vec.y);
}

Vector2D Vector2D::operator-(Vector2D vec)
{
	return Vector2D(x - vec.x, y - vec.y);
}

Vector2D Vector2D::operator-()
{
	return Vector2D(-x, -y);
}

void Vector2D::operator+=(Vector2D vec)
{
	x += vec.x;
	y += vec.y;
}

void Vector2D::operator-=(Vector2D vec)
{
	x -= vec.x;
	y -= vec.y;
}

void Vector2D::operator*=(float num)
{
	x *= num;
	y *= num;
}

SDL_Point* Vector2D::ToSDLPoint()
{
	SDL_Point* point = new SDL_Point();
	point->x = (int)x;
	point->y = (int)y;
	return point;
}

Vector2D::~Vector2D()
{
}
#pragma endregion

#pragma region VECTOR3D
Vector3D::Vector3D() : x(0), y(0), z(0)
{

}

Vector3D::Vector3D(float x, float y, float z) : x(x), y(y), z(z)
{
}

Vector3D::Vector3D(const Vector3D& vector) : x(vector.x), y(vector.y), z(vector.z)
{

}

float Vector3D::Magnitude()
{
	if (x == 0 && y == 0 && z == 0)
	{
		return 0.0f;
	}
	return std::sqrtf((x * x) + (y * y) + (z * z));
}

Vector3D Vector3D::Normalize()
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

float Vector3D::Dot(const Vector3D& v)
{
	return (x * v.x) + (y * v.y) + (z * v.z);
}

float Vector3D::Dot(Vector3D& v)
{
	return (x * v.x) + (y * v.y) + (z * v.z);
}

Vector3D Vector3D::Cross(const Vector3D& v1)
{
	Vector3D v;

	v.x = (y * v1.z) - (z * v1.y);
	v.y = (z * v1.x) - (x * v1.z);
	v.z = (x * v1.y) - (y * v1.x);

	return v;
}

float Vector3D::Distance(const Vector3D& v1, const Vector3D& v2)
{
	return std::sqrtf((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y) + (v1.z - v2.z) * (v1.z - v2.z));
}

float Vector3D::Dot(const Vector3D& v1, const Vector3D& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

Vector3D Vector3D::Cross(const Vector3D& v1, const Vector3D& v2)
{
	Vector3D v;

	v.x = (v1.y * v2.z) - (v1.z * v2.y);
	v.y = (v1.z * v2.x) - (v1.x * v2.z);
	v.z = (v1.x * v2.y) - (v1.y * v2.x);

	return v;
}

Vector3D Vector3D::Lerp(const Vector3D& start, const Vector3D& end, float delta)
{
	Vector3D v;

	v.x = start.x * (1.0f - delta) + end.x * (delta);
	v.y = start.y * (1.0f - delta) + end.y * (delta);
	v.z = start.z * (1.0f - delta) + end.z * (delta);

	return v;
}

Vector3D Vector3D::Perpendicular()
{
	return Vector3D(y, -x, z);
}

Vector3D Vector3D::operator*(float num)
{
	return Vector3D(x * num, y * num, z * num);
}

Vector3D Vector3D::operator/(float num)
{
	return Vector3D(num / x, num / y, num / z);
}

Vector3D Vector3D::operator+(Vector3D vec)
{
	return Vector3D(x + vec.x, y + vec.y, z + vec.z);
}

Vector3D Vector3D::operator-(Vector3D vec)
{
	return Vector3D(x - vec.x, y - vec.y, z - vec.z);
}

Vector3D Vector3D::operator-()
{
	return Vector3D(-x, -y, -z);
}

void Vector3D::operator+=(Vector3D vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
}

void Vector3D::operator+=(Vector2D vec)
{
	x += vec.x;
	y += vec.y;
}

void Vector3D::operator-=(Vector3D vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
}

void Vector3D::operator-=(Vector2D vec)
{
	x -= vec.x;
	y -= vec.y;
}

void Vector3D::operator*=(float num)
{
	x *= num;
	y *= num;
	z *= num;
}

Vector3D::~Vector3D()
{
}
#pragma endregion