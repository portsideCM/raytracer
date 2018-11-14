#pragma once
#include <math.h>

class Object;
typedef Vector Color;

class Vector
{
private:
	float m_x, m_y, m_z;
public:
	Vector() = default;
	Vector(float value);
	Vector(float xval, float yval, float zval);
	~Vector() = default;

	float getX() const { return m_x; }
	float getY() const { return m_y; }
	float getZ() const { return m_z; }

	Vector& operator*(const float c);
	Vector& operator*(const Vector& other);
	float operator%(const Vector& other) const;
	Vector& operator+ (const Vector& other);
	Vector& operator- (const Vector& other);
	Vector& operator^ (const Vector& other);

	Vector& power(const float exp);

	float mag2() const;
	float mag() const;

	Vector& normalize();
};

struct Ray
{
	Vector start, direction;

	Ray(Vector start, Vector dir) : start(start), direction(dir) {}
};

struct HitRecord
{
	float t;
	Vector point;
	Vector normal;
	const Object* obj;

	HitRecord() : t(0), point(), normal(), obj(nullptr) {}
};