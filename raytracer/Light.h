#pragma once
#include "stdafx.h"
#include "Vector.h"
class PointLight
{
private:
	Vector position;
	Color color;
	float intensity;
public:
	const double pi = 3.1415926535897;
	PointLight(Vector& pos, Color& c, float i);
	~PointLight();

	Vector getPos() { return position; }
	Color getColor() { return color; }
	float getIntensity() { return intensity; }

	float getIntensityAt(float distance) 
	{
		return (intensity / (pi * 4 * distance * distance));
	}
};

