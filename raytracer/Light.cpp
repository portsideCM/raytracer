#include "stdafx.h"
#include "Light.h"

PointLight::PointLight(Vector & pos, Color & c, float i)
	: position(pos), color(c), intensity(i)
{
}

PointLight::~PointLight()
{
}
