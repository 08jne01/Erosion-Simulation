#pragma once
#include "Entity.h"
#include "Camera.h"
#include "Config.h"
#include <glm/ext.hpp>

glm::mat4 makeModelMatrix(const Entity& entity);
glm::mat4 makeViewMatrix(const Camera& camera);
glm::mat4 makeProjectionMatrix(const Config& config);
