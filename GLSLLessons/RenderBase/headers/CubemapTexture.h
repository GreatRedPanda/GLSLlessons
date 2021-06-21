#pragma once
#include <glm.hpp>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include "Texture.h"
class CubemapTexture: Texture
{


public: 

	void loadCubemap(std::vector<std::string> texturesData);
	void bind(int n) override;
};

