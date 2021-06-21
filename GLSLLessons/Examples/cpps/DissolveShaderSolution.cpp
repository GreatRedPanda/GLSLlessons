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
#include "ShaderProgram.h"
#include "Mesh.h"
#include "CubeMeshPreset.h"
#include "Texture.h"
#include "Camera.h"
#include "Definitions.h"


void loopDissolve(GLFWwindow* window, Camera& mainCamera, RenderLoop timingCalc)
{




    ShaderProgram shaderProgram;
    shaderProgram.loadShaderFromFile("shaders/vertex.glsl", GL_VERTEX_SHADER);
    shaderProgram.loadShaderFromFile("shaders/Dissolve.glsl", GL_FRAGMENT_SHADER);
    shaderProgram.attachAndCompile();

    ShaderProgram shaderLightProgram;
    shaderLightProgram.loadShaderFromFile("shaders/vertex.glsl", GL_VERTEX_SHADER);
    shaderLightProgram.loadShaderFromFile("shaders/LightShader.glsl", GL_FRAGMENT_SHADER);
    shaderLightProgram.attachAndCompile();



    Mesh cube = Mesh(CubeMeshPreset::vertecies, CubeMeshPreset::normals, CubeMeshPreset::uvs);
    Mesh lightCube = Mesh(CubeMeshPreset::vertecies, CubeMeshPreset::normals, CubeMeshPreset::uvs);
    //  glm::vec3 lightCubePos = glm::vec3(-1, 1.5f, 2.2f);
     // glm::vec3 lightColor = glm::vec3(0.2f, 1.5f, 2.2f);
    Texture mainTex = Texture("texture.jpg");
    Texture noiseTex = Texture("2.jpg");
    glm::vec3 cubesPos[4] =
    {
    glm::vec3(-3.7f, 0.5f,0.2f),
    glm::vec3(-0.7f, 0.5f,0.2f),
    glm::vec3(-2.7f, 1.5f,0.2f),
      glm::vec3(2.7f, 0.5f,4.2f),
    };
    glm::vec3 lightCubesPos[4] =
    {
    glm::vec3(-13.7f, 0.5f,0.2f),
    glm::vec3(4.7f, 0.5f,0.2f),
    glm::vec3(12.7f, 1.5f,10.2f),
      glm::vec3(2.7f, 15.0f,4.2f),
    };
    glm::vec3 lightCubesColors[4] =
    {
    glm::vec3(0.7f, 0.5f,0.2f),
    glm::vec3(0.2f, 0.5f,0.4f),
    glm::vec3(0.1f, 0.5f,0.1f),
      glm::vec3(0.3f, 0.0f,0.3f),
    };
    float dt = 0;
    while (!glfwWindowShouldClose(window))
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        timingCalc(window);
        dt += 0.01;
        double now = glfwGetTime();
        glClearColor(0.5, 0.5, 0.5, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       /* for (size_t i = 0; i < 4; i++)
        {
            shaderLightProgram.use();
            glm::mat4 cubeModel = glm::mat4(1.0f);
            cubeModel = glm::translate(cubeModel, lightCubesPos[i]);
            glUniformMatrix4fv(shaderLightProgram.getUniform("model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
            glUniformMatrix4fv(shaderLightProgram.getUniform("view"), 1, GL_FALSE, &mainCamera.getView()[0][0]);
            glUniformMatrix4fv(shaderLightProgram.getUniform("projection"), 1, GL_FALSE, glm::value_ptr(mainCamera.projection));
            glUniform3f(shaderLightProgram.getUniform("LightSourceColor"), lightCubesColors[i].x, lightCubesColors[i].y, lightCubesColors[i].z);
            lightCube.Draw(shaderLightProgram);
        }*/

        for (size_t i = 0; i < 4; i++)
        {
            shaderProgram.use();

            glUniform1i(shaderProgram.getUniform("MainTex"), 0);
            glUniform1i(shaderProgram.getUniform("DissolveTex"), 1);
            mainTex.bind(0);
            noiseTex.bind(1);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubesPos[i]);
            glUniformMatrix4fv(shaderProgram.getUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(shaderProgram.getUniform("view"), 1, GL_FALSE, &mainCamera.getView()[0][0]);
            glUniformMatrix4fv(shaderProgram.getUniform("projection"), 1, GL_FALSE, glm::value_ptr(mainCamera.projection));
            glUniform3f(shaderProgram.getUniform("viewPos"), mainCamera.cameraPos.x, mainCamera.cameraPos.y, mainCamera.cameraPos.z);

            glUniform3f(shaderProgram.getUniform("lightPos"), lightCubesPos[i].x, lightCubesPos[i].y, lightCubesPos[i].z);
            glUniform3f(shaderProgram.getUniform("lightColor"), lightCubesColors[i].x, lightCubesColors[i].y, lightCubesColors[i].z);
            glUniform1f(shaderProgram.getUniform("Time"),now);
           
            cube.Draw(shaderProgram);

        }



        glfwSwapBuffers(window);
        glfwPollEvents();

    }



}