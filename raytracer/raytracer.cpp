// raytracer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BMPWriter.h"
#include "SceneRenderer.h"
#include <chrono>
#include <iostream>

constexpr auto HEIGHT = 2048;
constexpr auto WIDTH = 2048;
constexpr auto COLORS = 255;
constexpr int MAX_BOUNCES = 8;

void setupScene(Scene& s);
std::uint8_t* colorBufferToInt(Color colorBuffer[], std::size_t size);

int main(int argc, char** argv) 
{
	Scene s(Camera(WIDTH, HEIGHT));
	setupScene(s);
	Color* colorBuffer = new Color[HEIGHT * WIDTH];
	SceneRenderer renderer(s, Color(0.1f, 0.1f, 0.1f), MAX_BOUNCES);

	auto startTime = std::chrono::high_resolution_clock::now();
	renderer.renderScene(colorBuffer, HEIGHT, WIDTH);
	auto endTime = std::chrono::high_resolution_clock::now();
	std::cout << "Rendering took " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << " milliseconds\n";

	uint8_t* intBuffer = colorBufferToInt(colorBuffer, HEIGHT * WIDTH);

	std::fstream fs("out.bmp", std::fstream::out);
	BMPWriter writer(WIDTH, HEIGHT, fs, BMPWriter::BitsPerPixel::_24BPP);
	writer.writeImage(intBuffer, HEIGHT * WIDTH * 3, BMPWriter::DataType::RGB);
	fs.close();

	return 0;
}

inline int RGBClamp(float x)
{
	return 255 * fmaxf(0, fminf(1, x));
}

std::uint8_t* colorBufferToInt(Color colorBuffer[], std::size_t size)
{
	std::uint8_t* intBuffer = new std::uint8_t[size * 3];
	for (int i = 0; i < size; i++)
	{
		intBuffer[i * 3] = RGBClamp(colorBuffer[i].m_x);
		intBuffer[i * 3 + 1] = RGBClamp(colorBuffer[i].m_y);
		intBuffer[i * 3 + 2] = RGBClamp(colorBuffer[i].m_z);
	}
	delete[] colorBuffer;
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


