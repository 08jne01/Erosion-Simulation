#pragma once
#include <glm/ext.hpp>
#include "Entity.h"
class Camera

{
public:

	Camera();
	void cameraTranslate();
	void cameraRotate();
	void update();
	void setVel(int coord, double value);
	void setAngVel(double cursX, double cursY);

	float speed;

	Entity cameraEntity;

private:

	glm::vec3 velocity;
	glm::vec3 angularVelocity;
	float rotationSpeed, x, y, z;
	double defaultCursX, defaultCursY;
};