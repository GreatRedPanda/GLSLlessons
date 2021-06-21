#pragma once
#include <glm.hpp>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
class Camera
{


	float lastFrame = 0.0f;
	float yaw = -90, pitch;
	
	
public:
	glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 projection = glm::mat4(1.0);
	
	void setProjectionPerspective(float fovy, float aspect, float near, float far);
	void move(GLFWwindow &window, float deltaTime);
	void rotate(GLFWwindow& window, float mouseX, float lastMouseX, float mouseY, float lastMouseY );
	glm::vec3 getCameraAngles();
	glm::mat4 getView();
};

