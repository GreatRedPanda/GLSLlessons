#pragma once
#include <glm.hpp>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include <fstream> 
#include <istream> 
#include <iostream>
#include <string>

class ShaderProgram
{
	int program;
public:
	ShaderProgram();
	void addShader(std::string path, int shaderType);
	void compile();
	void use();

	int getUniform(std::string name);

};

