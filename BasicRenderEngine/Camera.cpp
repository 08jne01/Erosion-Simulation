#include "Camera.h"
#include <iostream>
Camera::Camera() : cameraEntity(), velocity(0.0), angularVelocity(0.0), speed(0.1), rotationSpeed(0.1), x(0.0), y(0.0), z(0.0)

{

}

void Camera::cameraTranslate()

{
	glm::mat4 matrix(1.0);
	matrix = glm::rotate(matrix, glm::radians(-cameraEntity.rotation.z), { 0, 0, 1 });
	matrix = glm::rotate(matrix, glm::radians(-cameraEntity.rotation.y), { 0, 1, 0 });
	matrix = glm::rotate(matrix, glm::radians(-cameraEntity.rotation.x), { 1, 0, 0 });

	glm::vec4 vec = matrix * glm::vec4(velocity, 1.0);

	cameraEntity.position = { cameraEntity.position.x + speed * vec.x, cameraEntity.position.y + speed * vec.y , cameraEntity.position.z + speed * vec.z };
}

void Camera::cameraRotate()

{
	cameraEntity.rotation = { cameraEntity.rotation.x + angularVelocity.x, cameraEntity.rotation.y + angularVelocity.y , cameraEntity.rotation.z + angularVelocity.z };
}

void Camera::setVel(int coord, double value)

{
	velocity[coord] = value;
}

void Camera::setAngVel(double cursX, double cursY)

{
	//std::cout << cursX << " " << cursY << std::endl;
	glm::vec3 rot = { rotationSpeed*(cursY - defaultCursY), rotationSpeed*(cursX - defaultCursX), 0.0 };
	cameraEntity.rotation += rot;
	defaultCursX = cursX;
	defaultCursY = cursY;
}

void Camera::update()

{
	cameraTranslate();
	cameraRotate();
	//std::cout << "\r" << cameraEntity.position.x << " " << cameraEntity.position.y << " " << cameraEntity.position.z; 
}