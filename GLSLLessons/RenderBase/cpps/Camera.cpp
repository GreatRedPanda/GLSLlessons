#include "Camera.h"

void Camera::setProjectionPerspective(float fovy, float aspect, float near, float far)
{
    projection = glm::perspective(fovy, aspect, near, far);
}

void Camera::move(GLFWwindow & window, float deltaTime)
{
    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(&window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(&window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(&window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(&window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void Camera::rotate(GLFWwindow& window, float mouseX, float lastMouseX, float mouseY, float lastMouseY)
{

    float xoffset = mouseX - lastMouseX;
    float yoffset = lastMouseY - mouseY;


    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

glm::vec3 Camera::getCameraAngles()
{
    return glm::vec3(pitch, yaw, 0);
}



glm::mat4 Camera::getView()
{
    return   glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
