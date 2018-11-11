#include "stdafx.h"
#include "Camera.h"


Camera::Camera(int width, int height)
	: widthPixels(width), heightPixels(height), fov(45.0f)
{
	aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	this->width = 2 * tan((pi * fov) / 360);
	this->height = this->width / aspectRatio;
	pixelWidth = this->width / widthPixels;
	pixelHeight = this->height / heightPixels;
}

Ray Camera::castRay(const int& x, const int& y) const
{
	float x_pos = (pixelWidth - width) / 2.0f + x * pixelWidth;
	float y_pos = (pixelHeight + height) / 2.0f - y * pixelHeight;
	return Ray(Vector(0, 0, 0), Vector(x_pos, y_pos, 1).normalize());
}
