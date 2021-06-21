#ifndef  AL
#define AL
 


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
#include "stb_image.h"
#include "SphereMesh.h"
ShaderProgram shaderProg;
ShaderProgram shaderLightProg;

AL::glm::vec3 cubesPositions[4] =
{
glm::vec3(0.0f, -0.5f, 0.0),
glm::vec3(2.0f, 1.0f, 1.0),
glm::vec3(-1.0f, 1.0f, 2.0),
  glm::vec3(-1.0f, 1.0f, 4.0),
};
glm::vec3 lightCubesPositions[1] =
{
glm::vec3(-2.0f, 4.0f, 0.1f)
};
glm::vec3 lightCubesColours[4] =
{
glm::vec3(0.0f, 0.0f,0.0f),
glm::vec3(0.2f, 0.5f,0.4f),
glm::vec3(0.1f, 0.5f,0.1f),
  glm::vec3(0.3f, 0.0f,0.3f),
};
Mesh cubeM;
void render(Camera& mainCamera);
//unsigned int depthMap;
//unsigned int depthMapFBO;



void loopAL(GLFWwindow* window, Camera& mainCamera, RenderLoop timingCalc)
{
    int shadowWidth = width;
    int shadowHeight = height;

    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);



#pragma region MainFrameBuffer
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion



#pragma region OutlineCustomBuffer
    unsigned int customColorBuffer;
    glGenFramebuffers(1, &customColorBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, customColorBuffer);
    // create a color attachment texture
    unsigned int texCustomColorbuffer;
    glGenTextures(1, &texCustomColorbuffer);
    glBindTexture(GL_TEXTURE_2D, texCustomColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texCustomColorbuffer, 0);
#pragma endregion

   

    ShaderProgram depthShader = ShaderProgram();
    depthShader.loadShaderFromFile("shaders/AdvancedLightning/ShadowProjV.glsl", GL_VERTEX_SHADER);
    depthShader.loadShaderFromFile("shaders/AdvancedLightning/DepthShaderFrag.glsl", GL_FRAGMENT_SHADER);
    depthShader.attachAndCompile();

    ShaderProgram testPlainShader = ShaderProgram();
    testPlainShader.loadShaderFromFile("shaders/VertexScreen.glsl", GL_VERTEX_SHADER);
    testPlainShader.loadShaderFromFile("shaders/AdvancedLightning/outlinePP.frag", GL_FRAGMENT_SHADER);
   // testPlainShader.loadShaderFromFile("shaders/TextureShader.glsl", GL_FRAGMENT_SHADER);
    testPlainShader.attachAndCompile();


    ShaderProgram portalShader = ShaderProgram();
    portalShader.loadShaderFromFile("shaders/AdvancedLightning/portal.vert", GL_VERTEX_SHADER);
    portalShader.loadShaderFromFile("shaders/AdvancedLightning/portal.frag", GL_FRAGMENT_SHADER);
    portalShader.attachAndCompile();

    ShaderProgram unlitShader = ShaderProgram();
    unlitShader.loadShaderFromFile("shaders/AdvancedLightning/ShadowProjV.glsl", GL_VERTEX_SHADER);
    unlitShader.loadShaderFromFile("shaders/AdvancedLightning/unlit.frag", GL_FRAGMENT_SHADER);
    unlitShader.attachAndCompile();

    shaderProg.loadShaderFromFile("shaders/AdvancedLightning/shadowmapper.vert", GL_VERTEX_SHADER);
    shaderProg.loadShaderFromFile("shaders/AdvancedLightning/shadowmapper.frag", GL_FRAGMENT_SHADER);
    shaderProg.attachAndCompile();


    shaderLightProg.loadShaderFromFile("shaders/vertex.glsl", GL_VERTEX_SHADER);
   // shaderLightProg.loadShaderFromFile("shaders/AdvancedLightning/outlinePP.frag", GL_FRAGMENT_SHADER);
    //shaderLightProg.loadShaderFromFile("shaders/LightShader.glsl", GL_FRAGMENT_SHADER);
    shaderLightProg.attachAndCompile();



    std::vector<float> quadVerticies = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
       // positions   // texCoords
       -1.0f,  1.0f,  0,
       -1.0f, -1.0f,  0,
        1.0f, -1.0f,  0,

       -1.0f,  1.0f,  0,
        1.0f, -1.0f,  0,
        1.0f,  1.0f, 0,
    };
    std::vector<float> quadUVs = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
      // positions   // texCoords
       0.0f, 1.0f,
       0.0f, 0.0f,
       1.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 0.0f,
         1.0f, 1.0f
    };
    Mesh quad = Mesh(quadVerticies, quadUVs);
    cubeM = Mesh(CubeMeshPreset::vertecies, CubeMeshPreset::normals, CubeMeshPreset::uvs);
 //   Mesh lightCube = Mesh(CubeMeshPreset::vertecies, CubeMeshPreset::normals, CubeMeshPreset::uvs);

    Texture mainT;
    Texture noiseT;
    mainT = Texture("texture.jpg");
    noiseT = Texture("2.jpg");

    SphereMesh sp = SphereMesh();


    Mesh sphere = Mesh(sp.vertecies, sp.normals, sp.uvs);
    float dt = 0;

    glm::mat4 modelPortal = glm::mat4(1.0f);
    modelPortal = glm::translate(modelPortal, glm::vec3(-1.5f, 1.4f, 1.4f));

    
    while (!glfwWindowShouldClose(window))
    {
        
        timingCalc(window);
        dt += 0.1;
        double now = glfwGetTime();
        lightCubesPositions[0] = mainCamera.cameraPos;
       glm::vec3 lightDir = glm::vec3(0, 0, 0);
      float near_plane = 1.0f, far_plane = 7.5f;
      glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
      glm::mat4 lightView = glm::lookAt(lightCubesPositions[0],mainCamera.cameraPos+mainCamera.cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));
      glm::mat4 lightSpaceMatrix = mainCamera.projection * lightView;



     
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, shadowWidth, shadowHeight);
        glClear(GL_DEPTH_BUFFER_BIT);
       
        for (size_t i = 0; i < 4; i++)
        {       
            depthShader.use();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubesPositions[i]);
            glUniformMatrix4fv(depthShader.getUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(depthShader.getUniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
            cubeM.Draw(depthShader);
        }
   




        //pass 2
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(0.0, 0.0, 0.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
        for (size_t i = 0; i < 4; i++)
        {
            shaderProg.use();
            glUniform1i(shaderProg.getUniform("diffuseTexture"), 0);
            glUniform1i(shaderProg.getUniform("shadowMap"), 1);
            mainT.bind(0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, depthMap);
           // noiseT.bind(1);
                          
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubesPositions[i]);
       /*     if (i == 0)
                model = glm::scale(model, glm::vec3(10, 1, 10));*/
            glUniformMatrix4fv(shaderProg.getUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(shaderProg.getUniform("view"), 1, GL_FALSE, &mainCamera.getView()[0][0]);
            glUniformMatrix4fv(shaderProg.getUniform("projection"), 1, GL_FALSE, glm::value_ptr(mainCamera.projection));
            glUniformMatrix4fv(shaderProg.getUniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

            glUniform3f(shaderProg.getUniform("viewPos"), mainCamera.cameraPos.x, mainCamera.cameraPos.y, mainCamera.cameraPos.z);
            glUniform3f(shaderProg.getUniform("lightPos"), lightCubesPositions[0].x, lightCubesPositions[0].y, lightCubesPositions[0].z);
          //  glUniform3f(shaderProg.getUniform("light.Color"), 1.0f, 1.0f,1.0f);
            cubeM.Draw(shaderProg);
        }
        portalShader.use();
        glUniform1i(shaderProg.getUniform("shadowMap"), 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glUniformMatrix4fv(portalShader.getUniform("model"), 1, GL_FALSE, glm::value_ptr(modelPortal));
        glUniformMatrix4fv(portalShader.getUniform("view"), 1, GL_FALSE, &mainCamera.getView()[0][0]);
        glUniformMatrix4fv(portalShader.getUniform("projection"), 1, GL_FALSE, glm::value_ptr(mainCamera.projection));
        glUniformMatrix4fv(portalShader.getUniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        glUniformMatrix4fv(portalShader.getUniform("cameraModel"), 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(1.0f),mainCamera.cameraPos)));
        glUniform3f(portalShader.getUniform("lightPos"),  lightCubesPositions[0].x, lightCubesPositions[0].y, lightCubesPositions[0].z);
        glUniform3f(portalShader.getUniform("viewPos"), mainCamera.cameraPos.x, mainCamera.cameraPos.y, mainCamera.cameraPos.z);
        sphere.Draw(portalShader);
      

        glBindFramebuffer(GL_FRAMEBUFFER, customColorBuffer);
        glClearColor(0.0, 0.0, 0.0, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (size_t i = 0; i < 4; i++)
        {
            unlitShader.use();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubesPositions[i]);
            glUniformMatrix4fv(unlitShader.getUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(unlitShader.getUniform("view"), 1, GL_FALSE, &mainCamera.getView()[0][0]);
            glUniformMatrix4fv(unlitShader.getUniform("projection"), 1, GL_FALSE, glm::value_ptr(mainCamera.projection));
            glUniformMatrix4fv(unlitShader.getUniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
            cubeM.Draw(unlitShader);
        }


       
//PostProcess     
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(0.0, 0.0, 0.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT );     
        testPlainShader.use();

        glUniform1i(testPlainShader.getUniform("mainTexture"), 0);
        glUniform1i(testPlainShader.getUniform("selectionObjTexture"), 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texCustomColorbuffer);
     
        quad.Draw(testPlainShader);
 
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
}


    void render(Camera & mainCamera)
    {
    
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (size_t i = 0; i < 1; i++)
        {
            shaderLightProg.use();
            glm::mat4 cubeModel = glm::mat4(1.0f);
            cubeModel = glm::translate(cubeModel, lightCubesPositions[i]);
            glUniformMatrix4fv(shaderLightProg.getUniform("model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
            glUniformMatrix4fv(shaderLightProg.getUniform("view"), 1, GL_FALSE, &mainCamera.getView()[0][0]);
            glUniformMatrix4fv(shaderLightProg.getUniform("projection"), 1, GL_FALSE, glm::value_ptr(mainCamera.projection));
            glUniform3f(shaderLightProg.getUniform("LightSourceColor"), lightCubesColours[i].x, lightCubesColours[i].y, lightCubesColours[i].z);
            cubeM.Draw(shaderLightProg);
        }

        for (size_t i = 0; i < 4; i++)
        {
          //  mainT.bind(0);
          //  noiseT.bind(1);
            shaderProg.use();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubesPositions[i]);    
        /*    if (i == 0)
                model = glm::scale(model, glm::vec3(10, 1, 10));*/
            glUniformMatrix4fv(shaderProg.getUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(shaderProg.getUniform("view"), 1, GL_FALSE, &mainCamera.getView()[0][0]);
            glUniformMatrix4fv(shaderProg.getUniform("projection"), 1, GL_FALSE, glm::value_ptr(mainCamera.projection));
            glUniform3f(shaderProg.getUniform("ViewPos"), mainCamera.cameraPos.x, mainCamera.cameraPos.y, mainCamera.cameraPos.z);
            glUniform3f(shaderProg.getUniform("light.Position"), lightCubesPositions[0].x, lightCubesPositions[0].y, lightCubesPositions[0].z);
            glUniform3f(shaderProg.getUniform("light.Ambient"), 0.2f, 0.2f, 0.2f);
            glUniform3f(shaderProg.getUniform("light.Diffuse"), 1, 1, 1);
            glUniform3f(shaderProg.getUniform("light.Specular"), 1, 1, 1);
            glUniform1f(shaderProg.getUniform("material.Shininess"), 64);
            glUniform1i(shaderProg.getUniform("material.Diffuse"), 0);
            glUniform1i(shaderProg.getUniform("material.Specular"), 1);


            cubeM.Draw(shaderProg);

        }
    }

#endif // ! AL