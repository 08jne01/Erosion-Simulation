#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

class BaseShader

{
public:

	BaseShader(unsigned int shaderType, std::string shaderFilename);

	unsigned int getID();
	void deleteShader();

private:

	void readSource();
	void compile();

	unsigned int type;
	unsigned int id;
	std::string source;
	std::string filename;
};
