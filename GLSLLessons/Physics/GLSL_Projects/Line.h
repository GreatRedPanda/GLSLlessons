#pragma once
#include <glm.hpp>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
class Line
{
	glm::vec2 start;
	glm::vec2 end;
	float pointSize;
	float max(float x, float y);
	float min(float x, float y);
public:
	Line(glm::vec2 start, glm::vec2 end, float pointSize);
	Line();
	
	void draw();
	bool thc(float x, float y);
};

