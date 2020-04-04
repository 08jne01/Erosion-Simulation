#pragma once
#include <algorithm>
#include "BaseRenderer.h"
#include "PerlinNoise.h"
#include "Color.h"

class Mesh : public BaseRenderer

{
public:
	Mesh();
	void errode();
	void makeRadiusPoints(float radius);
	int index(int i, int j);
	int randomInt(int min, int max);
	void makeHeightMap();
	void buildMesh();
	void calcGrad();
	void setAllUniforms(const Entity& entity, const Camera& camera, float gst, float gba);
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	float lerp(float p1, float p2, float w);
	float interp(float w);
	Color curve(float grad);

	struct HeightAndGradient

	{
		float gradX, gradZ, height;
	};

	HeightAndGradient gradient(float x, float z);

private:

	int mapSizeX = 256;
	int mapSizeZ = 256;

	int xSize;
	int zSize;

	float meshResolution = 1.0;
	PerlinNoise noise;
	std::vector<std::vector<int>> radiusPoints;
	std::vector<std::vector<float>> radiusPointsWeights;
	std::vector<float> heightMap;

	Color grass = { 29.0 / 255.0, 168.0 / 255.0, 29.0 / 255.0 };
	Color soil = { 148.0 / 255.0, 76.0 / 255.0, 13.0 / 255.0 };
};