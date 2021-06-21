
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

#include "CubemapTexture.h"
void loopSkybox(GLFWwindow* window, Camera& mainCamera, RenderLoop timingCalc)
{
    std::vector<std::string> cubemaps =
    {
    "2.jpg","2.jpg","2.jpg",  "2.jpg","2.jpg","2.jpg"
    };
    CubemapTexture cm = CubemapTexture();
    cm.loadCubemap(cubemaps);


    ShaderProgram skyBoxShader;
    skyBoxShader.loadShaderFromFile("shaders/Skybox/SkyboxVertex.glsl", GL_VERTEX_SHADER);
    skyBoxShader.loadShaderFromFile("shaders/Skybox/SkyboxFrag.glsl", GL_FRAGMENT_SHADER);
    skyBoxShader.attachAndCompile();
    ShaderProgram shaderProgram;
    shaderProgram.loadShaderFromFile("shaders/vertex.glsl", GL_VERTEX_SHADER);
    shaderProgram.loadShaderFromFile("shaders/MultipleLightning.glsl", GL_FRAGMENT_SHADER);
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

    std::vector<float> skyboxVertices = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
   
    Mesh skyboxCube = Mesh(skyboxVertices);
    
    float dt = 0;
   
    
    
    
    
    
    while (!glfwWindowShouldClose(window))
    {
      //  glEnable(GL_BLEND);
      //  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        timingCalc(window);
        dt += 0.1;
        double now = glfwGetTime();
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthMask(GL_FALSE);
      
        skyBoxShader.use();
        cm.bind(0);
        glm::mat4 view = glm::mat4(glm::mat3(mainCamera.getView()));
        glUniformMatrix4fv(skyBoxShader.getUniform("view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(skyBoxShader.getUniform("projection"), 1, GL_FALSE, glm::value_ptr(mainCamera.projection));
        skyboxCube.Draw(skyBoxShader);

        glDepthMask(GL_TRUE);
     

        for (size_t i = 0; i < 4; i++)
        {
            shaderLightProgram.use();
            glm::mat4 cubeModel = glm::mat4(1.0f);
            cubeModel = glm::translate(cubeModel, lightCubesPos[i]);
            glUniformMatrix4fv(shaderLightProgram.getUniform("model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
            glUniformMatrix4fv(shaderLightProgram.getUniform("view"), 1, GL_FALSE, &mainCamera.getView()[0][0]);
            glUniformMatrix4fv(shaderLightProgram.getUniform("projection"), 1, GL_FALSE, glm::value_ptr(mainCamera.projection));
            glUniform3f(shaderLightProgram.getUniform("LightSourceColor"), lightCubesColors[i].x, lightCubesColors[i].y, lightCubesColors[i].z);
            lightCube.Draw(shaderLightProgram);
        }

        for (size_t i = 0; i < 4; i++)
        {
            mainTex.bind(0);
            noiseTex.bind(1);
            shaderProgram.use();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubesPos[i]);
            glUniformMatrix4fv(shaderProgram.getUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(shaderProgram.getUniform("view"), 1, GL_FALSE, &mainCamera.getView()[0][0]);
            glUniformMatrix4fv(shaderProgram.getUniform("projection"), 1, GL_FALSE, glm::value_ptr(mainCamera.projection));
            glUniform3f(shaderProgram.getUniform("viewPos"), mainCamera.cameraPos.x, mainCamera.cameraPos.y, mainCamera.cameraPos.z);

            glUniform3f(shaderProgram.getUniform("dirLight.direction"), mainCamera.cameraFront.x, mainCamera.cameraFront.y, mainCamera.cameraFront.z);
            glUniform3f(shaderProgram.getUniform("dirLight.ambient"), 0.2f, 0.2f, 0.2f);
            glUniform3f(shaderProgram.getUniform("dirLight.diffuse"), 1, 1, 1);
            glUniform3f(shaderProgram.getUniform("dirLight.specular"), 1, 1, 1);

            glUniform1f(shaderProgram.getUniform("material.shininess"), 64);
            glUniform1i(shaderProgram.getUniform("material.diffuse"), 0);
            glUniform1i(shaderProgram.getUniform("material.specular"), 1);





            glUniform3f(shaderProgram.getUniform("pointLights[0].position"), lightCubesPos[0].x, lightCubesPos[0].y, lightCubesPos[0].z);
            glUniform3f(shaderProgram.getUniform("pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
            glUniform3f(shaderProgram.getUniform("pointLights[0].diffuse"), lightCubesColors[0].x, lightCubesColors[0].y, lightCubesColors[0].z);
            glUniform3f(shaderProgram.getUniform("pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
            glUniform1f(shaderProgram.getUniform("pointLights[0].constant"), 1.0f);
            glUniform1f(shaderProgram.getUniform("pointLights[0].linear"), 0.09);
            glUniform1f(shaderProgram.getUniform("pointLights[0].quadratic"), 0.032);
            // point light 2
            glUniform3f(shaderProgram.getUniform("pointLights[1].position"), lightCubesPos[1].x, lightCubesPos[1].y, lightCubesPos[1].z);
            glUniform3f(shaderProgram.getUniform("pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
            glUniform3f(shaderProgram.getUniform("pointLights[1].diffuse"), lightCubesColors[1].x, lightCubesColors[1].y, lightCubesColors[1].z);
            glUniform3f(shaderProgram.getUniform("pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
            glUniform1f(shaderProgram.getUniform("pointLights[1].constant"), 1.0f);
            glUniform1f(shaderProgram.getUniform("pointLights[1].linear"), 0.09);
            glUniform1f(shaderProgram.getUniform("pointLights[1].quadratic"), 0.032);
            // point light 3
            glUniform3f(shaderProgram.getUniform("pointLights[2].position"), lightCubesPos[2].x, lightCubesPos[2].y, lightCubesPos[2].z);
            glUniform3f(shaderProgram.getUniform("pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
            glUniform3f(shaderProgram.getUniform("pointLights[2].diffuse"), lightCubesColors[2].x, lightCubesColors[2].y, lightCubesColors[2].z);
            glUniform3f(shaderProgram.getUniform("pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
            glUniform1f(shaderProgram.getUniform("pointLights[2].constant"), 1.0f);
            glUniform1f(shaderProgram.getUniform("pointLights[2].linear"), 0.09);
            glUniform1f(shaderProgram.getUniform("pointLights[2].quadratic"), 0.032);
            // point light 4
            glUniform3f(shaderProgram.getUniform("pointLights[3].position"), lightCubesPos[3].x, lightCubesPos[3].y, lightCubesPos[3].z);
            glUniform3f(shaderProgram.getUniform("pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
            glUniform3f(shaderProgram.getUniform("pointLights[3].diffuse"), lightCubesColors[3].x, lightCubesColors[3].y, lightCubesColors[3].z);
            glUniform3f(shaderProgram.getUniform("pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
            glUniform1f(shaderProgram.getUniform("pointLights[3].constant"), 1.0f);
            glUniform1f(shaderProgram.getUniform("pointLights[3].linear"), 0.09);
            glUniform1f(shaderProgram.getUniform("pointLights[3].quadratic"), 0.032);



            cube.Draw(shaderProgram);

        }



        glfwSwapBuffers(window);
        glfwPollEvents();

    }



}
