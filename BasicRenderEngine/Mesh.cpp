#include "Mesh.h"

Mesh::Mesh(): noise(1, 1, 34, 30.0, 5.0, 2.0, 0.50)

{
	xSize = mapSizeX / meshResolution;
	zSize = mapSizeZ / meshResolution;
	
}

int Mesh::index(int i, int j)

{
	if (i >= 0 && j >= 0 && i < xSize && j < zSize)

	{
		return i + xSize * j;
	}

	return -1;
}

Mesh::HeightAndGradient Mesh::gradient(float posX, float posZ)

{
	int coordX = (int)posX;
	int coordZ = (int)posZ;

	float x = posX - coordX;
	float z = posZ - coordZ;

	int nodeIndexNW = coordZ * xSize + coordX;

	float nw = 0.0, ne = 0.0, sw = 0.0, se = 0.0;

	int n = index(coordX, coordZ);
	if (n >= 0) nw = heightMap[n];
	n = index(coordX + 1, coordZ);
	if (n >= 0) ne = heightMap[n];
	n = index(coordX, coordZ + 1);
	if (n >= 0) sw = heightMap[n];
	n = index(coordX + 1, coordZ + 1);
	if (n >= 0) se = heightMap[n];

	float gradX = (ne - nw) * (1 - z) + (se - sw) * z;
	float gradZ = (sw - nw) * (1 - x) + (se - ne) * x;

	float height = nw * (1 - x) * (1 - z) + ne * x * (1 - z) + sw * (1 - x) * z + se * x * z;
	HeightAndGradient result;
	result.gradX = gradX;
	result.gradZ = gradZ;
	result.height = height;

	return result;
}

void Mesh::makeRadiusPoints(float radius)

{
	for (int i = 0; i < heightMap.size(); i++)

	{
		std::vector<int> brushIndexPoints;
		std::vector<float> brushIndexWeights;
		int xPos = i % xSize;
		int zPos = i / xSize;
		float totalWeight = 0.0;

		for (int x = -(int)radius; x <= (int)radius; x++)

		{
			for (int y = -(int)radius; y <= (int)radius; y++)

			{
				int otherX = xPos + x;
				int otherZ = zPos + y;

				float dist = sqrt((float)(xPos - otherX) * (xPos - otherX) + (float)(zPos - otherZ) * (zPos - otherZ));

				if (dist < radius)

				{
					int n = index(otherX, otherZ);
					float amount = pow(1 - dist / radius, 0.5);
					if (n >= 0) brushIndexPoints.push_back(n), brushIndexWeights.push_back(amount), totalWeight += amount;
				}
			}
		}

		for (int i = 0; i < brushIndexWeights.size(); i++)

		{
			brushIndexWeights[i] /= totalWeight;
		}

		radiusPointsWeights.push_back(brushIndexWeights);
		radiusPoints.push_back(brushIndexPoints);
	}
}

void Mesh::errode()

{
	
	float evapourateSpeed = 0.01;
	float gravity = 4;
	float inertia = 0.1;
	float minSedimentCapacity = 0.01;
	float erodeSpeed = 0.3;
	float depositSpeed = 0.01;
	float capacityFactor = 1.0;
	float maxSpeed = 10.0;

	int its = 100;
	int droplets = 10;
	for (int i = 0; i < droplets; i++)

	{
		float posX = randomInt(0, xSize - 1);
		float posZ = randomInt(0, zSize - 1);

		float dirX = 0;
		float dirZ = 0;
		float speed = 1.0;
		float water = 1.0;
		float sediment = 0.0;

		for (int j = 0; j < its; j++)

		{
			HeightAndGradient heightAndGradient = gradient(posX, posZ);
			dirX = (dirX * inertia - heightAndGradient.gradX * (1 - inertia));
			dirZ = (dirZ * inertia - heightAndGradient.gradZ * (1 - inertia));
			float currentHeight = heightAndGradient.height;
			float size = sqrt(dirX * dirX + dirZ * dirZ);

			int coordX = (int)posX;
			int coordZ = (int)posZ;
			float x = posX - coordX;
			float z = posZ - coordZ;

			if (size != 0.0)

			{
				dirX /= size;
				dirZ /= size;
			}

			posX += dirX;
			posZ += dirZ;

			float newHeight = gradient(posX, posZ).height;
			float deltaHeight = newHeight - currentHeight;

			//std::cout << "Water " << water << std::endl;
			//std::cout << "Speed " << speed << std::endl;


			float sedimentCapacity = std::max(-deltaHeight * speed * water * capacityFactor, minSedimentCapacity);

			//std::cout << deltaHeight << std::endl;
			//std::cout << -deltaHeight * speed * water * depositSpeed << std::endl;

			if (coordX > 0 && coordX < xSize - 2 && coordZ > 0 && coordZ < zSize - 2)

			{
				if (sediment > sedimentCapacity || deltaHeight > 0)

				{
					float depositAmount = (deltaHeight > 0) ? std::min(deltaHeight, sediment) : (sediment - sedimentCapacity) * depositSpeed;
					sediment -= depositAmount;

					int n = index(coordX, coordZ);
					if (n >= 0) heightMap[n] += depositAmount * (1 - x) * (1 - z);
					n = index(coordX + 1, coordZ);
					if (n >= 0) heightMap[n] += depositAmount * (x) * (1 - z);
					n = index(coordX, coordZ + 1);
					if (n >= 0) heightMap[n] += depositAmount * (1 - x) * (z);
					n = index(coordX + 1, coordZ + 1);
					if (n >= 0) heightMap[n] += depositAmount * (x) * (z);

					/*int n = index(coordX, coordZ);

					if (n >= 0)

					{

						for (int k = 0; k < radiusPoints[n].size(); k++)

						{
							float part = radiusPointsWeights[n][k] * depositAmount;
							heightMap[radiusPoints[n][k]] += part;
							sediment -= part;
						}
					}*/
				}

				else

				{
					float amountToErode = std::min((sedimentCapacity - sediment) * erodeSpeed, -deltaHeight);

					//std::cout << "Sediment " << sediment << std::endl;
					//std::cout << "Capacity " << sedimentCapacity << std::endl;
					//std::cout << "DeltaHeight " << deltaHeight << std::endl;

					float actualErodeAmount = amountToErode;

					int n = index(coordX, coordZ);

					if (n >= 0)

					{
						float deltaErode = 0.0;

						for (int k = 0; k < radiusPoints[n].size(); k++)

						{
							float part = radiusPointsWeights[n][k] * amountToErode;
							if (part > heightMap[radiusPoints[n][k]]) heightMap[radiusPoints[n][k]] = 0;
							else heightMap[radiusPoints[n][k]] -= part;
							sediment += part;
						}
					}



					//std::cout << (sedimentCapacity - sediment) * erodeSpeed << std::endl;


					/*if (n >= 0)

					{
						if (heightMap[n] < amountToErode) actualErodeAmount = heightMap[n];
						else actualErodeAmount = amountToErode;
						heightMap[n] -= actualErodeAmount * (1 - x) * (1 - z);
					}
					n = index(coordX + 1, coordZ);
					if (n >= 0)

					{
						if (heightMap[n] < amountToErode) actualErodeAmount = heightMap[n];
						else actualErodeAmount = amountToErode;
						heightMap[n] -= amountToErode * (x) * (1 - z);
					}

					n = index(coordX, coordZ + 1);
					if (n >= 0)

					{
						if (heightMap[n] < amountToErode) actualErodeAmount = heightMap[n];
						else actualErodeAmount = amountToErode;
						heightMap[n] -= amountToErode * (1 - x) * (z);
					}

					n = index(coordX + 1, coordZ + 1);
					if (n >= 0)

					{
						if (heightMap[n] < amountToErode) actualErodeAmount = heightMap[n];
						else actualErodeAmount = amountToErode;
						heightMap[n] -= amountToErode * (x) * (z);
					}*/

					//sediment += amountToErode;
				}
			}
			speed = sqrt(speed*speed + gravity*deltaHeight);
			speed = std::min(maxSpeed, speed);
			water *= (1 - evapourateSpeed);
		}
	}
}

int Mesh::randomInt(int min, int max)

{
	return rand() % (max - min + 1) + min;
}

void Mesh::makeHeightMap()

{
	int min = 10000.0;
	for (int i = 0; i < xSize; i++)

	{
		for (int j = 0; j < zSize; j++)

		{
			float height = noise.fractalNoise((float)i * meshResolution, (float)j * meshResolution, 5, 256.0);
			heightMap.push_back(height);
			if (height < min) min = height;
		}
	}

	for (int i = 0; i < heightMap.size(); i++)

	{
		heightMap[i] -= min;
	}

	makeRadiusPoints(2.0 / meshResolution);
}

void Mesh::buildMesh()

{
	vertices.clear();
	indices.clear();

	for (int i = 0; i < xSize; i++)

	{
		for (int j = 0; j < zSize; j++)

		{
			float height = heightMap[i + xSize*j];
			Vertex newVertex({ i * meshResolution, height, j *meshResolution}, { 0.1, 1.0, 0.1});
			vertices.push_back(newVertex);
		}
	}

	for (int i = 0; i < xSize - 1; i++)

	{
		for (int j = 0; j < zSize - 1; j++)

		{
			indices.push_back(i + j*xSize);
			indices.push_back(i + 1 + (j + 1) * xSize);
			indices.push_back(i + (j + 1) * xSize);
			indices.push_back(i + j*xSize);
			indices.push_back(i + 1 + j*xSize);
			indices.push_back(i + 1 + (j + 1) * xSize);
		}
		
	}
}

void Mesh::setAllUniforms(const Entity& entity, const Camera& camera, float gst, float gba)

{
	setUniforms(entity, camera);
	int GST = glGetUniformLocation(shaderProgram, "GrassSlopeThreshold");
	glUniform1f(GST, gst);

	int GBA = glGetUniformLocation(shaderProgram, "GrassBlendAmount");
	glUniform1f(GBA, gba);
}

void Mesh::calcGrad()

{
	float max = -1.0;
	float min = 10000.0;
	for (int i = 1; i < xSize - 1; i++)

	{
		for (int j = 1; j < zSize - 1; j++)

		{
			/*Vertex current = vertices[i + xSize * j];
			Vertex plusX = vertices[i + 1 + xSize * j];
			Vertex plusZ = vertices[i + xSize * (j + 1)];
			Vertex minusX = vertices[i - 1 + xSize * j];
			Vertex minusZ = vertices[i + xSize * (j - 1)];

			float gradX = (plusX.position.y - minusX.position.y) / (2*meshResolution);
			float gradZ = (plusZ.position.y - minusZ.position.y) / (2*meshResolution);

			float totalGradient = sqrt(gradX * gradX + gradZ * gradZ);
			if (totalGradient > max) max = totalGradient;
			if (totalGradient < min) min = totalGradient;

			vertices[i + xSize * j].color.g = totalGradient;*/

			glm::vec3 normal;

			glm::vec3 side1 = vertices[i + 1 + xSize * j].position - vertices[i + xSize * j].position;
			glm::vec3 side2 = vertices[i + 1 + xSize * (j + 1)].position - vertices[i + xSize * j].position;

			normal = glm::cross(side1, side2);
			normal = glm::normalize(normal);
			//std::cout << normal.y << std::endl;
			vertices[i + xSize * j].normal = normal;
		}
	}
}

Color Mesh::curve(float grad)

{
	Color color;
	color.r = 0.5*lerp(grass.r, soil.r, interp(grad)) + 0.5*lerp(grass.r, soil.r, grad);
	color.g = 0.5 * lerp(grass.g, soil.g, interp(grad)) + 0.5*lerp(grass.g, soil.g, grad);
	color.b = 0.5 * lerp(grass.b, soil.b, interp(grad)) + 0.5*lerp(grass.b, soil.b, grad);
	return color;
}

float Mesh::interp(float w)

{
	return (tanh(20*(w - 0.4) / 0.25) + 1) / 2.0;
}

float Mesh::lerp(float p1, float p2, float w)

{
	return p1 + w * (p2 - p1);
}