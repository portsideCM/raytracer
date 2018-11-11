// raytracer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "BMPWriter.h"
#include <chrono>
#include <iostream>

constexpr auto HEIGHT = 8;
constexpr auto WIDTH = 8;
constexpr auto COLORS = 255;
constexpr float Inf = std::numeric_limits<float>::max();
constexpr float EPSILON = 0.0001;
constexpr int MAX_BOUNCES = 2;

struct Scene
{
	std::vector<Object*> objects;
	std::vector<PointLight*> lights;
	Camera c;
	Color ambientLight;

	Scene(Camera cam) : c(cam), objects(), lights(), ambientLight(0.1f, 0.1f, 0.1f) {}

	HitRecord findClosestIntersect(const Ray& ray) const {
		HitRecord currIntersect;
		HitRecord closestIntersect;
		closestIntersect.t = std::numeric_limits<float>::min();
		for each(Object* o in objects)
		{
			if (o->intersects(ray, currIntersect)) {
				if (currIntersect.t < closestIntersect.t)
					closestIntersect = currIntersect;
			}
		}
		return currIntersect;
	}
};

void setupScene(Scene& s);
void renderScene(Scene& s, Color colorBuffer[]);
std::uint8_t* colorBufferToInt(Color colorBuffer[], std::size_t size);
Color trace(const Scene& s, Ray& ray, int bouncesLeft);
Color illuminate(const Scene& s, HitRecord& hr, Vector& rayOrigin);
Color tonemap(Color& linearRGB);

int main(int argc, char** argv) 
{
	Scene s(Camera(WIDTH, HEIGHT));
	setupScene(s);
	Color* colorBuffer = new Color[HEIGHT * WIDTH];

	auto startTime = std::chrono::high_resolution_clock::now();
	renderScene(s, colorBuffer);
	auto endTime = std::chrono::high_resolution_clock::now();
	std::cout << "Rendering took " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << " milliseconds\n";

	uint8_t* intBuffer = colorBufferToInt(colorBuffer, HEIGHT * WIDTH);

	std::fstream fs("out.bmp", std::fstream::out);
	BMPWriter writer(WIDTH, HEIGHT, fs, BMPWriter::BitsPerPixel::_24BPP);
	writer.writeImage(intBuffer, HEIGHT * WIDTH * 3, BMPWriter::DataType::RGB);
	fs.close();

	return 0;
}

void renderScene(Scene& s, Color colorBuffer[]) 
{
	for (int y = 0; y < HEIGHT; y++) 
	{
		for (int x = 0; x < WIDTH; x++) 
		{
			//Create ray from eye through pixel
			Ray createdRay = s.c.castRay(x, y);

			//Set the current color to the object's color
			colorBuffer[y * HEIGHT + x] = /*tonemap*/(trace(s, createdRay, MAX_BOUNCES));
		}
	}
}

Color trace(const Scene& s, Ray& ray, int bouncesLeft)
{
	HitRecord closestIntersect = s.findClosestIntersect(ray);
	if (closestIntersect.obj == nullptr)
		return Color(0.01f);

	Color directColor = illuminate(s, closestIntersect, ray.start);

	if (bouncesLeft == 0 || closestIntersect.obj->getReflectivity() == 0.0f)
		return directColor;
	else
	{
		Vector rayDir = ray.direction;
		Vector bounceReflection = rayDir - (closestIntersect.normal * ((closestIntersect.normal%rayDir) * 2.0f));
		Ray bounceRay = Ray(closestIntersect.point + (bounceReflection * EPSILON), bounceReflection);
		Color reflectColor = trace(s, bounceRay, bouncesLeft - 1);
		
		directColor = directColor * (1 - closestIntersect.obj->getReflectivity());
		reflectColor = reflectColor * closestIntersect.obj->getReflectivity();

		return (directColor + reflectColor);
	}
}

Color illuminate(const Scene& s, HitRecord& hr, Vector& rayOrigin)
{
	return hr.obj->getColor();
	Color colorToReturn(0);
	colorToReturn = colorToReturn + (hr.obj->getColor() * s.ambientLight);

	for each(PointLight* l in s.lights)
	{
		float dist = (l->getPos() - hr.point).mag();
		Vector L = (l->getPos() - hr.point).normalize();

		Ray shadowRay = Ray((L * EPSILON) + hr.point, L);
		HitRecord closestInter = s.findClosestIntersect(shadowRay);

		if (closestInter.obj != nullptr && closestInter.t < dist)
			continue;

		Color C_diff = hr.obj->getColor();
		Color C_spec = l->getColor();
		Color I = l->getIntensityAt(dist);

		Vector V = (hr.point - rayOrigin).normalize();
		Vector R = L - (hr.normal * (L%hr.normal * 2.0f));

		float diffuseProjection = hr.normal%L;
		if (diffuseProjection > 0)
			colorToReturn = colorToReturn + (C_diff * I * (diffuseProjection * hr.obj->getPhongKD()));

		float spectralProjection = R % V;
		if (spectralProjection > 0) {
			spectralProjection = powf(spectralProjection, hr.obj->getAlpha());
			colorToReturn = colorToReturn + (C_spec * I * (spectralProjection * hr.obj->getPhongKS()));
		}
	}
	
	return colorToReturn;
}

Color tonemap(Color& linearRGB) {
	float invGamma = 1.0f / 2.2f;
	float a = 2.0f;  // controls brightness
	float b = 1.3f; // controls contrast

	// Sigmoidal tone mapping
	Color powRGB = linearRGB.power(b);
	Color displayRGB = powRGB * (powRGB + powf(0.5f / a, b)).inv();

	// Display encoding - gamma
	Color gammaRGB = displayRGB.power(invGamma);
	return gammaRGB;
}

inline int RGBClamp(float x)
{
	return 255 * fmaxf(0, fminf(1, x));
}

std::uint8_t* colorBufferToInt(Color colorBuffer[], std::size_t size)
{
	std::uint8_t* intBuffer = new std::uint8_t[size * 3];
	for (int i = 0; i < size; ++i)
	{
		intBuffer[i] = RGBClamp(colorBuffer[i].r);
		intBuffer[i + 1] = RGBClamp(colorBuffer[i].g);
		intBuffer[i + 2] = RGBClamp(colorBuffer[i].b);
	}
	return intBuffer;
}

void setupScene(Scene& s) 
{
	/*
	Plane* plane = new Plane(Vector(0.0f, 4.22f, -2.18f), Vector(0.0f, -0.10622, 4.68013), Color(.06f,.06f,.06f), 0.1f, 0.6f, 0.0f, 0.0f);
	Sphere* sphere = new Sphere(Vector(0.72724f, -0.35322f, 3.199f), 0.45f, Color(0.23f, 0.69f, 0.062f), 0.3f, 0.8f, 1.2f, 10.0f);
	PointLight* light1 = new PointLight(Vector(2.0f, 2.0f, 4.5f), Color(1.0f, 1.0f, 1.0f), 80.0f);
	PointLight* light2 = new PointLight(Vector(-2.0f, 2.5f, 1.0f), Color(0.69f, 0.69f, 1.0f), 50.0f);
	s.objects.push_back(plane);
	s.objects.push_back(sphere);
	s.lights.push_back(light1);
	s.lights.push_back(light2);
	*/
	s.objects.push_back(new Sphere(Vector(0.55, -0.16, 3.5), 0.5, Color(0.0f, 0.44f, 0.8f), 0.3f, 0.8f, 1.2f, 10.0f));
	s.objects.push_back(new Sphere(Vector(-0.55, 0, 5), 0.9, Color(1.0f, 0.12f, 0.15f), 0.3f, 0.8f, 1.2f, 10.0f));
	s.lights.push_back(new PointLight(Vector(1, 3, 2), Color(0.76f, 0.93f, 1.0f), 80.0f));
	s.lights.push_back(new PointLight(Vector(-5, 1, 4), Color(1.0f, 0.75f, 0.76f), 80.0f));
}


