#pragma once
#include <glm/ext.hpp>
struct Vertex

{
	Vertex(glm::vec3 position_, glm::vec3 normal_):
		position(position_), normal(normal_)

	{

	}

	glm::vec3 position;
	glm::vec3 normal;
};