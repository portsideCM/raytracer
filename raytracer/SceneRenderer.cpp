#include "stdafx.h"
#include "SceneRenderer.h"

Color SceneRenderer::trace(Ray& ray, const int bouncesLeft)
{
	if (_scene.findClosestIntersect(ray, _hr)) 
	{
		Color objectColor = _hr.obj->getColor();
		Color directIllum = illuminate(_hr, ray.start);

		float reflectivity = _hr.obj->getReflectivity();
		if (reflectivity == 0.0f || bouncesLeft == 0)
			return directIllum;
		else
		{
			Vector bounceDir = ray.direction.reflectThisAcross(_hr.normal);
			Ray bounceRay(_hr.point + (bounceDir * 0.001f), bounceDir);
			Color reflectColor = trace(bounceRay, bouncesLeft - 1);
			directIllum * (1.0f - reflectivity);
			reflectColor * reflectivity;
			return directIllum + reflectColor;
		}
	}
	else return _backgroundColor;
}

Color SceneRenderer::illuminate(HitRecord & hr, Vector & rayOrigin)
{
	Color ret(0);
	const Object* obj = hr.obj;

	Color currObjColor = obj->getColor();

	ret + (_scene.ambientLight * currObjColor); //Is this right?

	HitRecord tempHit;
	for each(PointLight* currLight in _scene.lights)
	{
		float dist = (currLight->getPos() - hr.point).mag();
		Vector lightVec = (currLight->getPos() - hr.point).normalize();

		Ray shadowRay(hr.point + (lightVec * 0.001f), lightVec);
		if (_scene.findClosestIntersect(shadowRay, tempHit) && tempHit.t < dist)
			continue;

		Color I = currLight->getColor() * currLight->getIntensityAt(dist);
		Color lightColor = currLight->getColor();

		float diffuseProjection = hr.normal%lightVec;
		if (diffuseProjection > 0)
			ret + (currObjColor * I * (diffuseProjection * obj->getPhongKD()));
	}

	return ret;
}


SceneRenderer::SceneRenderer(Scene & s, Color& background, int numBounces)
	:_scene(s), _bounces(numBounces), _hr(), _backgroundColor(background)
{
	if (_bounces <= 0)
		throw new std::invalid_argument("Number of bounces must be at least 1");
}

void SceneRenderer::renderScene(Color colorBuffer[], size_t height, size_t width)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			//Create ray from eye through pixel
			Ray createdRay = _scene.castRay(x, y);

			//Set the current color to the object's color
			colorBuffer[y * height + x] = trace(createdRay, _bounces);
		}
	}
}
