#include "stdafx.h"
#include "Vector.h"

Vector::Vector(float value)
	: m_x(value), m_y(value), m_z(value)
{
}

Vector::Vector(float xval, float yval, float zval)
	: m_x(xval), m_y(yval), m_z(zval)
{
}

Vector Vector::operator*(const float c) const
{
	return Vector(c * m_x, c * m_y, c * m_z);
}

Vector Vector::operator*(const Vector & other) const
{
	return Vector(m_x * other.m_x, m_y * other.m_y, m_z * other.m_z);
}

float Vector::operator%(const Vector& other) const
{
	return (m_x * other.m_x) + (m_y * other.m_y) + (m_z * other.m_z);
}

Vector Vector::operator+(const Vector& other) const
{
	return Vector(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
}

Vector Vector::operator-(const Vector& other) const
{
	return Vector(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
}

Vector Vector::operator^(const Vector& other) const
{
	return Vector(m_y * other.m_z - m_z * other.m_y, m_z * other.m_x - m_x * other.m_z, m_x * other.m_y - m_y * other.m_x);
}

float Vector::mag2() const
{
	return static_cast<float>((m_x * m_x) + (m_y * m_y) + (m_z * m_z));
}

float Vector::mag() const
{
	return sqrtf(mag2());
}

Vector& Vector::normalize()
{
	float magnitude = mag();
	if (magnitude == 0)
	{
		m_x = 0;
		m_y = 0;
		m_z = 0;
	}
	else 
	{
		m_x /= magnitude;
		m_y /= magnitude;
		m_z /= magnitude;
	}
	return *this;
}



