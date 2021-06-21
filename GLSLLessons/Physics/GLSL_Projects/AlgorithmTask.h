#pragma once
#include <glm.hpp>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
class AlgorithmTask
{

public:
	virtual void loop( GLFWwindow &window) = 0;
};

