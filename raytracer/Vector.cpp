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

Vector& Vector::operator*(const float c)
{
	m_x *= c;
	m_y *= c;
	m_z *= c;
	return *this;
}

Vector& Vector::operator*(const Vector & other)
{
	m_x *= other.m_x;
	m_y *= other.m_y;
	m_z *= other.m_z;
	return *this;
}

float Vector::operator%(const Vector& other) const
{
	return (m_x * other.m_x) + (m_y * other.m_y) + (m_z * other.m_z);
}

Vector& Vector::operator+(const Vector& other)
{
	m_x += other.m_x;
	m_y += other.m_y;
	m_z += other.m_z;
	return *this;
}

Vector& Vector::operator-(const Vector& other)
{
	m_x -= other.m_x;
	m_y -= other.m_y;
	m_z -= other.m_z;
	return *this;
}

Vector& Vector::operator^(const Vector& other)
{
	m_x = m_y * other.m_z - m_z * other.m_y;
	m_y = m_z * other.m_x - m_x * other.m_z;
	m_z = m_x * other.m_y - m_y * other.m_x;
	return *this;
}

Vector& Vector::power(const float exp)
{
	m_x = powf(m_x, exp);
	m_y = powf(m_y, exp);
	m_z = powf(m_z, exp);
	return *this;
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



