#pragma once
#include <glm.hpp>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
class SphereMesh
{



public:
	SphereMesh();
	 std::vector<float>  vertecies;
	   std::vector<float> normals;
	   std::vector<float> uvs;

};

