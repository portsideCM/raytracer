#include "stdafx.h"
#include "SceneRenderer.h"

Color SceneRenderer::trace(Ray & ray, const int bouncesLeft)
{
	if (_scene.findClosestIntersect(ray, _hr)) 
	{

	}
	else return _backgroundColor;
}

Color SceneRenderer::illuminate(HitRecord & hr, Vector & rayOrigin)
{
	return Color();
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
