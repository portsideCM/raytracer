#pragma once
#include "Vector.h"

class Camera
{
private:
	const float pi = 3.1415926535897;
	int widthPixels, heightPixels;
	float width, height;
	float fov;
	float aspectRatio;
	float pixelWidth, pixelHeight;
public:
	Camera(int width, int height);
	~Camera() = default;

	Ray castRay(const int& x, const int& y) const;
};

