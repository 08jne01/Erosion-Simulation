#pragma once
#include <vector>
#include <functional>
#include <iostream>
#include <assert.h>
#define PI 3.14159

class PerlinNoise

{
public:

	PerlinNoise(int xSize, int ySize, int seed_, int noise_, double scale, double lac_, double pers_);
	double perlin(double x, double y, double period);
	double fractalNoise(double x, double y, int layers, double period);
	double gradient(int value, float min, float max);

private:
	int xS, yS;
	double s;
	int seed, noise;
	double lac = 1.5;
	double pers = 0.4;
	double lerp(double p1, double p2, float w);
	double dotGridGradient(int intX, int intY, double x, double y);
	double lerpCos(double d1, double d2, double w);
};