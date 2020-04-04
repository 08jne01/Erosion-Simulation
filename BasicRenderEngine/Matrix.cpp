#include "Matrix.h"

glm::mat4 makeModelMatrix(const Entity& entity)

{
	glm::mat4 matrix(1.0);

	matrix = glm::translate(matrix, entity.position);

	matrix = glm::rotate(matrix, glm::radians(entity.rotation.x), { 1, 0, 0 });
	matrix = glm::rotate(matrix, glm::radians(entity.rotation.y), { 0, 1, 0 });
	matrix = glm::rotate(matrix, glm::radians(entity.rotation.z), { 0, 0, 1 });

	return matrix;
}

glm::mat4 makeViewMatrix(const Camera& camera)

{
	glm::mat4 matrix(1.0);

	matrix = glm::rotate(matrix, glm::radians(camera.cameraEntity.rotation.x), { 1, 0, 0 });
	matrix = glm::rotate(matrix, glm::radians(camera.cameraEntity.rotation.y), { 0, 1, 0 });
	matrix = glm::rotate(matrix, glm::radians(camera.cameraEntity.rotation.z), { 0, 0, 1 });

	matrix = glm::translate(matrix, camera.cameraEntity.position);

	return matrix;
}

glm::mat4 makeProjectionMatrix(const Config& config)

{
	float x = (float)config.width;
	float y = (float)config.height;
	float fieldOfView = glm::radians((float)config.fieldOfView);
	return glm::perspective(fieldOfView, x / y, 0.1f, 2000.0f);
}
