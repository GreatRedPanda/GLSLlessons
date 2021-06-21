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

#include <cstdlib>
#include <ctime>
#include "fluid.h"
#include <math.h>
#include "ShaderProgram.h"
#include "AlgorithmTask.h"
class CirclesFloates :AlgorithmTask
{

public:
	void loop(GLFWwindow& window) override;

};

