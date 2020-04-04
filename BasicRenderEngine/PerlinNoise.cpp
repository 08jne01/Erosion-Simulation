#include "PerlinNoise.h"

#include "PerlinNoise.h"

PerlinNoise::PerlinNoise(int xSize, int ySize, int seed_, int noise_, double scale, double lac_, double pers_) : 
	xS(xSize), yS(ySize), s(scale), seed(seed_), noise(noise_), lac(lac_), pers(pers_)

{
	//srand(seed);

	//std::cout << grad[10][10][0];
}

double PerlinNoise::lerp(double p1, double p2, float w)

{
	//return (1.0 - w) * p1 + (w * p2);
	return p1 + w * (p2 - p1);
}

double PerlinNoise::lerpCos(double d1, double d2, double w)

{
	double w2 = (1 - cos(w*PI)) / 2.0;
	return (d1 * (1 - w2) + d2 * w2);
}

double PerlinNoise::dotGridGradient(int intX, int intY, double x, double y)

{
	double dx = x - (double)intX;
	double dy = y - (double)intY;

	return (dx * (double)gradient(intX*intY, -noise, noise) + dy * (double)gradient(intX + intY, -noise, noise));
}

double PerlinNoise::perlin(double x, double y, double period)

{
	double locX = x / period;
	double locY = y / period;

	int x0 = floor(locX);
	//std::cout << x0 << std::endl;
	//x0 = x0 % (int)period;
	//std::cout << x0 << std::endl;
	int x1 = x0 + 1;
	int y0 = floor(locY);
	//y0 = y0 % (int)period;
	int y1 = y0 + 1;

	double sx = locX - (double)x0;
	double sy = locY - (double)y0;

	double n0, n1, ix0, ix1, value;
	n0 = dotGridGradient(x0, y0, locX, locY);
	n1 = dotGridGradient(x1, y0, locX, locY);
	ix0 = lerpCos(n0, n1, sx);
	n0 = dotGridGradient(x0, y1, locX, locY);
	n1 = dotGridGradient(x1, y1, locX, locY);
	ix1 = lerpCos(n0, n1, sx);

	value = lerpCos(ix0, ix1, sy);

	return s * value;
}

double PerlinNoise::fractalNoise(double x, double y, int layers, double period)

{
	double val = 0.0;

	for (int i = 0; i < layers; i++)

	{
		val += perlin(x + 12.0*seed, y - 4.0*seed, period*pow(lac, -i)) * pow(pers, i);
	}
	return val;
}

double PerlinNoise::gradient(int value, float min, float max)

{
	assert(min < max);
	std::hash<int> hash;
	float frac = (float)hash(value + seed) / (float)std::numeric_limits<std::size_t>::max();
	float diff = max - min;
	return diff * frac + min;
}