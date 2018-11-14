#include "stdafx.h"
#include "Object.h"

Object::Object(Vector& c, Color& col, float ref, float kd, float ks, float a)
	: center(c), color(col), reflectivity(ref), p_kD(kd), p_kS(ks), p_alpha(a)
{
}

Plane::Plane(Vector& n, Vector& p, Color& c, float ref, float kd, float ks, float a)
	: Object(p, c, ref, kd, ks, a), normal(n)
{
	normal.normalize();
}

Vector Plane::getNormalAt(const Vector& position) const
{
	return normal;
}

bool Plane::intersects(const Ray& ray, HitRecord& hr) const
{
	float directionNorm = normal % (ray.direction);
	if (!directionNorm)
		return false;
	float inter = -(center%normal + normal % ray.start) / (directionNorm);
	if (inter >= 0) {
		hr.t = inter;
		hr.normal = normal;
		hr.point = (ray.start + (ray.direction * inter));
		hr.obj = this;
		return true;
	}

	return false;
}

Sphere::Sphere(Vector& pos, float radius, Color& c, float ref, float kd, float ks, float a)
	: Object(pos, c, ref, kd, ks, a), radius(radius)
{
}

Vector Sphere::getNormalAt(const Vector& position) const
{
	return (position - center).normalize();
}

bool Sphere::intersects(const Ray& ray, HitRecord& hr) const
{
	float a = ray.direction%ray.direction;
	Vector oToCenter = ray.start - center;
	float b = 2 * (ray.direction%oToCenter);
	float c = oToCenter % oToCenter - (radius * radius);
	float det = b * b - 4 * a * c; //Wait to square root until we check if it's going to intersect
	
	if (det < 0)
	{
		return false;
	}
	
	det = sqrtf(det);
	float s;
	if (det == 0) 
	{
		s = -b / (2 * a);
		if (s < 0)
			return false;
	}
	else 
	{
		float s1 = (-b + det) / (2 * a);
		float s2 = (-b - det) / (2 * a);
		if (s1 < 0 && s2 < 0)
			return false;

		s = (s2 < 0 || (s1 < s2 && s1 >= 0)) ? s1 : s2;
	}

	hr.point = ray.start + ray.direction * s;
	hr.normal = getNormalAt(hr.point);
	hr.t = s;
	hr.obj = this;
	return true;
}
