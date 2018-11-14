#pragma once
#include "Vector.h"
#include "Object.h"
#include "Light.h"
#include "Camera.h"

struct Scene
{
	std::vector<Object*> objects;
	std::vector<PointLight*> lights;
	Camera c;
	Color ambientLight;

	Scene(Camera cam) : c(cam), objects(), lights(), ambientLight(0.1f) {}

	bool findClosestIntersect(const Ray& ray, HitRecord& hr) const {
		bool intersected = false;
		HitRecord currIntersect;
		HitRecord closestIntersect;
		closestIntersect.t = INFINITY;
		for each(Object* o in objects)
		{
			if (o->intersects(ray, currIntersect)) {
				if (currIntersect.t < closestIntersect.t) {
					hr = currIntersect;
					intersected = true;
				}
			}
		}
		return intersected;
	}

	Ray castRay(const int& x, const int& y) const {
		return c.castRay(x, y);
	}
};

class SceneRenderer
{
private:
	Scene& _scene;
	const int _bounces;
	HitRecord _hr;
	Color& _backgroundColor;
	Color trace(Ray& ray, int bouncesLeft);
	Color illuminate(HitRecord& hr, Vector& rayOrigin);
public:
	SceneRenderer() = delete;
	~SceneRenderer() = default;
	SceneRenderer(Scene& s, Color& background, int numBounces);
	void renderScene(Color colorBuffer[], size_t height, size_t width);
};

