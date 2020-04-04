#pragma once
#include <glm/ext.hpp>
struct Entity

{
	Entity(): position(glm::vec3(0.0)), rotation(glm::vec3(0.0)), velocity(glm::vec3(0.0))

	{

	}

	Entity(const glm::vec3 position_, const glm::vec3 rotation_): 
		position(position_), rotation(rotation_), velocity(glm::vec3(0.0))

	{

	}

	Entity(const glm::vec3 position_, const glm::vec3 rotation_, const glm::vec3 velocity_): 
		position(glm::vec3(0.0)), rotation(glm::vec3(0.0)), velocity(velocity_)

	{

	}

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 velocity;
};