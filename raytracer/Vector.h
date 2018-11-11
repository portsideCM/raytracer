#pragma once
#include <math.h>

class Object;

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

	Vector operator*(const float c) const;
	Vector operator*(const Vector& other) const;
	float operator%(const Vector& other) const;
	Vector operator+ (const Vector& other) const;
	Vector operator- (const Vector& other) const;
	Vector operator^ (const Vector& other) const;

	float mag2() const;
	float mag() const;

	Vector& normalize();
};

struct Ray
{
	Vector start, direction;

	Ray(Vector start, Vector dir) : start(start), direction(dir) {}
};

struct Color
{
	float r, g, b;
	Color(float red, float green, float blue) : r(red), g(green), b(blue) {}
	Color(float val) : r(val), g(val), b(val) {}
	Color() : r(0), g(0), b(0) {}

	Color& operator* (const float factor)
	{
		r *= factor;
		g *= factor;
		b *= factor;
		return *this;
	}

	Color operator+ (const Color& other) const
	{
		return Color(r + other.r, g + other.g, b + other.b);
	}

	Color operator+ (const float& other) const
	{
		return Color(r + other, g + other, b + other);
	}

	Color operator* (const Color& other) const
	{
		return Color(r * other.r, g * other.g, b * other.b);
	}
	 
	Color inv() const
	{
		return Color(1.0f / r, 1.0f / g, 1.0f / b);
	}

	Color power(const float& p) const
	{
		return Color(powf(r, p), powf(g, p), powf(b, p));
	}
};

struct HitRecord
{
	float t;
	Vector point;
	Vector normal;
	const Object* obj;

	HitRecord() : t(0), point(), normal(), obj(nullptr) {}
};