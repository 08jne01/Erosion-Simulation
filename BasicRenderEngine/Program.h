#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "BaseRenderer.h"
#include "Mesh.h"
#include "BaseShader.h"
#include "EventHandler.h"
#include "Camera.h"
#include "Clock.h"

class Program

{
public:
	Program(int width_, int height_);
	~Program();
	int mainLoop();

	static void framebuffer_size_callback(GLFWwindow* window, int width_, int height_);
	static void key_callback(GLFWwindow* window, int button, int scancode, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
private:
	int width, height;
	GLFWwindow* window;
	EventHandler eventHandler;
	Camera camera;
	float GrassBlendAmount = 0.35;
	float GrassGrassSlopeThreshold = 0.35;
};