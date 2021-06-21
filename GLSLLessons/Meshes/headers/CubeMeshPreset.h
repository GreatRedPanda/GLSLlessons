#pragma once
#include "Mesh.h"
#include <glm.hpp>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
class CubeMeshPreset
{

public:
    static  std::vector<float>  vertecies;
    static   std::vector<float> normals;
    static   std::vector<float> uvs;
};