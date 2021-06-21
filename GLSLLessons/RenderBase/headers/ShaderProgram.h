#pragma once
#include <istream> 
#include <iostream>
#include <string>
#include <glm.hpp>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include "Texture.h"
#include <fstream> 
#include <istream> 
#include <iostream>
#include <string>
//#include "OpenGLHeaders.h"
class ShaderProgram
{

private:

	int shaderProgram;

	std::vector<int> shaders = std::vector<int>();
public:
	void setSampler(std::string name, unsigned int samplerIndex, Texture& sampler);
	void loadShaderFromFile(std::string filePath, GLuint shaderType);
	void setUniformMat4(std::string name, glm::mat4 value);
	void setObjectMPV(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	void setUniformV1(std::string name, float value);
	void setUniformV3(std::string name, glm::vec3 value);
	void attachAndCompile();
	void use();
	

	unsigned int getUniform(std::string name);
};

