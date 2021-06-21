#ifndef  POST_PR
#define POST_PR


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


ShaderProgram shaderPostPrProgram;
ShaderProgram shaderProgram;
ShaderProgram shaderLightProgram;
Texture mainTex;
Texture noiseTex = Texture("2.jpg");
Mesh cube=Mesh() ;
Mesh lightCube = Mesh();

glm::vec3 cubesPos[4]  =
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
void render(GLFWwindow* window, Camera& mainCamera, glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos);
void loopBloom(GLFWwindow* window, Camera& mainCamera, RenderLoop timingCalc)
{
    mainTex = Texture();
    mainTex.loadTexture("texture.jpg");
  Texture  tt = Texture();
    tt.loadTexture("texture.jpg");
    lightCube = Mesh(CubeMeshPreset::vertecies, CubeMeshPreset::normals, CubeMeshPreset::uvs);
    cube = Mesh(CubeMeshPreset::vertecies, CubeMeshPreset::normals, CubeMeshPreset::uvs);
    //Framebuffers
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 720, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    //unsigned int rbo;
    //glGenRenderbuffers(1, &rbo);
    //glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 720, 720); // use a single renderbuffer object for both a depth AND stencil buffer.
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // --FRAMEBUFFERS

    
   shaderPostPrProgram.loadShaderFromFile("shaders/vertex.glsl", GL_VERTEX_SHADER);
    shaderPostPrProgram.loadShaderFromFile("shaders/TextureShader.glsl", GL_FRAGMENT_SHADER);
    shaderPostPrProgram.attachAndCompile();

   
    shaderProgram.loadShaderFromFile("shaders/vertex.glsl", GL_VERTEX_SHADER);
    shaderProgram.loadShaderFromFile("shaders/MultipleLightning.glsl", GL_FRAGMENT_SHADER);
    shaderProgram.attachAndCompile();

   
    shaderLightProgram.loadShaderFromFile("shaders/vertex.glsl", GL_VERTEX_SHADER);
    shaderLightProgram.loadShaderFromFile("shaders/LightShader.glsl", GL_FRAGMENT_SHADER);
    shaderLightProgram.attachAndCompile();


    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
       // positions   // texCoords
       -1.0f,  1.0f,  0,
       -1.0f, -1.0f,  0,
        1.0f, -1.0f,  0,

       -1.0f,  1.0f,  0,
        1.0f, -1.0f,  0,
        1.0f,  1.0f, 0,
    };
    float quadUVs[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
      // positions   // texCoords
       0.0f, 1.0f,
       0.0f, 0.0f,
       1.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 0.0f,
         1.0f, 1.0f
    };
    unsigned int quadVAO, quadVBO, quadUV;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadUV);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, quadUV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadUVs), &quadUVs, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    

    glm::mat4 m = glm::mat4(1.0f);
    m = glm::translate(m, glm::vec3(0.0f, 2.0f,4.0f));
   
   

    while (!glfwWindowShouldClose(window))
    {

        glm::vec3 reflCamPos = glm::reflect(mainCamera.cameraPos, glm::vec3(1.0f, 2.0f, 2.0f));
        reflCamPos.x = -mainCamera.cameraPos.x;
        reflCamPos.y = mainCamera.cameraPos.y;
        reflCamPos.z = mainCamera.cameraPos.z;
    
        
        glm::vec3 reflCamFront = glm::reflect(mainCamera.cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));
        reflCamFront.x =mainCamera.cameraFront.x;
        reflCamFront.y = mainCamera.cameraFront.y;
        reflCamFront.z = -mainCamera.cameraFront.z;

      //  lightCubesPos[0] = reflCamPos;
        //glm::vec3 angles = mainCamera.getCameraAngles();

        //angles.x + 180;
        //glm::vec3 direction;
        //direction.x = cos(glm::radians(angles.x)) * cos(glm::radians(angles.y));
        //direction.y = sin(glm::radians(angles.y));
        //direction.z = sin(glm::radians(angles.x)) * cos(glm::radians(angles.y));
        //reflCamFront = glm::normalize(direction);


        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(reflCamPos, reflCamPos + reflCamFront, mainCamera.cameraUp);

        timingCalc(window);
        double now = glfwGetTime();

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.7, 0.4f, 0.4f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        render(window, mainCamera, view, mainCamera.projection, reflCamPos);
      // render(window, mainCamera, mainCamera.getView(), mainCamera.projection, mainCamera.cameraPos);


   
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.3f, 0.4f, 0.4f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   
        glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0  
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        shaderPostPrProgram.use();    
        glBindVertexArray(quadVAO);
        glUniformMatrix4fv(shaderPostPrProgram.getUniform("model"), 1, GL_FALSE, glm::value_ptr(m));
        glUniformMatrix4fv(shaderPostPrProgram.getUniform("view"), 1, GL_FALSE, &mainCamera.getView()[0][0]);
        glUniformMatrix4fv(shaderPostPrProgram.getUniform("projection"), 1, GL_FALSE, glm::value_ptr(mainCamera.projection));
        
        glDrawArrays(GL_TRIANGLES, 0, 6);

        render(window, mainCamera, mainCamera.getView(), mainCamera.projection, mainCamera.cameraPos);


    


       


        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}
    void render(GLFWwindow* window, Camera& mainCamera, glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos)
    {

        for (size_t i = 0; i < 4; i++)
        {
            shaderLightProgram.use();
            glm::mat4 cubeModel = glm::mat4(1.0f);
            cubeModel = glm::translate(cubeModel, lightCubesPos[i]);
            glUniformMatrix4fv(shaderLightProgram.getUniform("model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
            glUniformMatrix4fv(shaderLightProgram.getUniform("view"), 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(shaderLightProgram.getUniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
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
            glUniformMatrix4fv(shaderProgram.getUniform("view"), 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(shaderProgram.getUniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
            glUniform3f(shaderProgram.getUniform("viewPos"), viewPos.x, viewPos.y, viewPos.z);

            glUniform3f(shaderProgram.getUniform("dirLight.direction"), viewPos.x, viewPos.y, viewPos.z);
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


    }

#endif