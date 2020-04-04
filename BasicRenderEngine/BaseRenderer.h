#pragma once
#include <vector>
#include <glad/glad.h>
#include "Vertex.h"
#include "Matrix.h"
#include "BufferID.h"

class BaseRenderer

{
public:
	BaseRenderer();
	~BaseRenderer();

	void makeBuffers(BufferID& buffersID);
	void updateBuffers(BufferID& buffer, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
	void setUniforms(const Entity& entity, const Camera& camera);
	void draw(const BufferID& buffer, unsigned int indicesSize);

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int shaderProgram;

	Config config;
};