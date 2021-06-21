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
#include "Line.h"
#include <math.h>
class SoftBody
{

private:
	float bodyRadius=0.2;
	glm::vec2 centerPosition;
	glm::vec2 velocity;
	glm::vec2 bodyVelocity;
	std::vector<glm::vec2> positions;
	std::vector<glm::vec2> velocities;
	float pressure = 0;
	float vel = 0;
	float sprLen[36][36];
	float k = 0.00058;
public:
	glm::vec3 color;
	SoftBody();
	SoftBody(glm::vec2 center);
	void simulate(float deltaTime);
	void draw();
	void checkCollisions(int position, const std::vector<SoftBody *> others);
	void checkCollisionsStatic(int position,  Line l);
	void checkBounds();
	glm::vec2 checCollision(glm::vec2 curPos,const SoftBody& other, glm::vec2 &closestPoint);

	static std::vector<SoftBody *> bodies ;
	Line lineTest;
};

