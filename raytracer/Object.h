#pragma once
#include "Vector.h"

class Object
{
protected:
	Vector center;
	Color color;
	float reflectivity;
	float p_kD, p_kS, p_alpha;
public:
	Object();
	Object(Vector& c);
	Object(Vector& c, Color& col, float ref, float kd, float ks, float a);
	virtual ~Object() {};

	void move(Vector& newPos) { center = newPos; }
	void setReflectivity(double newR) { reflectivity = newR; }
	void setColor(Color& newColor) { color = newColor; };

	Vector getPosition() const { return center; }
	Color getColor() const { return color; }
	float getReflectivity() const { return reflectivity; }
	float getPhongKD() const { return p_kD; }
	float getPhongKS() const { return p_kS; }
	float getAlpha() const { return p_alpha; }

	virtual bool intersects(const Ray& ray, HitRecord& hr) const = 0;
	virtual Vector getNormalAt(const Vector& position) const = 0;
};

class Plane : public Object
{
private:
	Vector normal;
public:
	Plane(Vector& n, Vector& p);
	Plane(Vector& n, Vector& p, Color& c, float ref, float kd, float ks, float a);

	Vector getNormalAt(const Vector& position) const;
	bool intersects(const Ray& ray, HitRecord& hr) const;
};

class Sphere : public Object 
{
private:
	float radius;
public:
	Sphere(Vector& pos, float radius, Color& c, float ref, float kd, float ks, float a);
	Sphere(Vector& pos, float radius);
	Sphere(Vector& pos);

	Vector getNormalAt(const Vector& position) const;
	bool intersects(const Ray& ray, HitRecord& hr) const;
};

