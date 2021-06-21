// GlfwTezst.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "OpenGLHeaders.h"
#include "Scene.h"
#include "RenderersHandler.h"
#include "DissolveExample.h"
#include "Outline_and_ForceField_Example.h"

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float lastX, lastY;
bool firstMouse;
Camera mainCamera;
void loop(GLFWwindow* window, Camera& mainCamera, RenderLoop timingCalc);
void loopBloom(GLFWwindow* window, Camera& mainCamera, RenderLoop timingCalc);
void loopSkybox(GLFWwindow* window, Camera& mainCamera, RenderLoop timingCalc);
void loopAL(GLFWwindow* window, Camera& mainCamera, RenderLoop timingCalc);
void loopDissolve(GLFWwindow* window, Camera& mainCamera, RenderLoop timingCalc);
void loopRayTracing(GLFWwindow* window, Camera& mainCamera, RenderLoop timingCalc);

int main()
{

    mainCamera.setProjectionPerspective(glm::radians(60.0f),(float)width/(float)height, 0.1f, 100.0f);

    //==========================INIT WINDOW======================================================================
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
   // glfwSetKeyCallback(window, processInput);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    GLenum err = glewInit();
    glEnable(GL_DEPTH_TEST); 



    Scene newScene = Scene();
   
    DissolveExample de = DissolveExample();
    Outline_and_ForceField_Example oe = Outline_and_ForceField_Example();
    newScene.sceneRenderersHandlers.push_back(&oe);
    newScene.loop(window, mainCamera, processInput);
//  loop(window, mainCamera, processInput);
   // loopBloom(window, mainCamera, processInput);
   // loopDissolve(window, mainCamera, processInput);
   // loopAL(window, mainCamera, processInput);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
     mainCamera.move(*window, deltaTime*2.5f);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    mainCamera.rotate(*window, xpos, lastX, ypos, lastY);
   
    lastX = xpos;
    lastY = ypos;

   
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
