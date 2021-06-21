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
struct MeshForBuffer
{
    glm::mat4 Model;
    std::vector<glm::vec3> vertecies;

};
void loopRayTracing(GLFWwindow* window, Camera& mainCamera, RenderLoop timingCalc)
{
   // unsigned int framebuffer;
  //  glGenFramebuffers(1, &framebuffer);
 //   glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
   // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    Texture sb = Texture("2.jpg");
    
    std::vector<std::string> cubemaps =
    {
    "2.jpg","2.jpg","2.jpg",  "2.jpg","2.jpg","2.jpg"
    };
    CubemapTexture cm = CubemapTexture();
    cm.loadCubemap(cubemaps);

    ShaderProgram testPlainShader = ShaderProgram();
    testPlainShader.loadShaderFromFile("shaders/VertexScreen.glsl", GL_VERTEX_SHADER);
    testPlainShader.loadShaderFromFile("shaders/TextureShader.glsl", GL_FRAGMENT_SHADER);
    testPlainShader.attachAndCompile();
    ShaderProgram skyBoxShader;
    skyBoxShader.loadShaderFromFile("shaders/Skybox/SkyboxVertex.glsl", GL_VERTEX_SHADER);
    skyBoxShader.loadShaderFromFile("shaders/Skybox/SkyboxFrag.glsl", GL_FRAGMENT_SHADER);
    skyBoxShader.attachAndCompile();


    ShaderProgram shaderRayTracingProgram;
    shaderRayTracingProgram.loadShaderFromFile("shaders/RayTracingShader.glsl", GL_COMPUTE_SHADER);
    //shaderRayTracingProgram.loadShaderFromFile("shaders/Dissolve.glsl", GL_FRAGMENT_SHADER);
    shaderRayTracingProgram.attachAndCompile();

    ShaderProgram shaderLightProgram;
    shaderLightProgram.loadShaderFromFile("shaders/vertex.glsl", GL_VERTEX_SHADER);
    shaderLightProgram.loadShaderFromFile("shaders/TextureShader.glsl", GL_FRAGMENT_SHADER);
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
    glm::vec3(1, 0.5f,3.2f),
    glm::vec3(2, 0.5f,0.2f),
    glm::vec3(3, 0.5f,1),
      glm::vec3(4, 0.5f,4.2f),
    };
    glm::vec3 lightCubesColors[4] =
    {
    glm::vec3(0.7f, 0.5f,0.2f),
    glm::vec3(0.2f, 0.5f,0.4f),
    glm::vec3(0.1f, 0.5f,0.1f),
      glm::vec3(0.3f, 0.0f,0.3f),
    };
    float dt = 0;

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
    std::vector<float> quadVerticies = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // positions   // texCoords
    -1.0f,  1.0f,  0,
    -1.0f, -1.0f,  0,
     1.f, -1.0f,  0,

    -1.0f,  1.0f,  0,
     1.f, -1.0f,  0,
    1.f,  1.0f, 0,
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
   
   
    std::vector<glm::vec4> vertecies = std::vector<glm::vec4>();
    std::vector<glm::vec4> normals = std::vector<glm::vec4>();
    for (size_t i = 0; i < CubeMeshPreset::vertecies.size(); i+=3)
    {
        normals.push_back(glm::vec4(CubeMeshPreset::normals[i],
            CubeMeshPreset::normals[i + 1],
            CubeMeshPreset::normals[i + 2],0));
        vertecies.push_back(glm::vec4(CubeMeshPreset::vertecies[i],
            CubeMeshPreset::vertecies[i + 1],
            CubeMeshPreset::vertecies[i + 2], 1));
    }
  
    std::vector<glm::mat4> meshes = std::vector<glm::mat4>();
   
    for (size_t i = 0; i < 3; i++)
    {

        for (size_t j = 0; j < 3; j++)
        {
            
          glm::vec3 pos = glm::vec3(i * 3, 0,j * 3);
           glm::mat4 modelMesh=  glm::translate(glm::mat4(1.0f), pos);         
           meshes.push_back(modelMesh);
        }

    }



    unsigned int VBO, VBO2, NBO;
    glGenBuffers(1, &VBO);
   
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, meshes.size() * sizeof(glm::mat4), &meshes[0], GL_STATIC_DRAW);
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, vertecies.size() * sizeof(glm::vec4), &vertecies[0], GL_STATIC_DRAW);
    glGenBuffers(1, &NBO);
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec4), &normals[0], GL_STATIC_DRAW);

    float sum = 0;
    
    while (!glfwWindowShouldClose(window))
    {
        sum += 0.1f;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (sum >= 1)
        {
            sum = 0;



            shaderRayTracingProgram.use();
            glm::mat4 cameraModel = glm::mat4(1.0f);

            glm::mat4 cameraRot = glm::mat4(1.0f);


            cameraRot = glm::rotate(cameraRot, mainCamera.getCameraAngles().y * 3.14f / 180, glm::vec3(0, 1, 0));
            cameraRot = glm::rotate(cameraRot, mainCamera.getCameraAngles().x * 3.14f / 180, glm::vec3(1, 0, 0));


            cameraRot = glm::translate(cameraRot, mainCamera.cameraPos);

            cameraModel = mainCamera.getView();
            cameraModel = glm::translate(cameraModel, mainCamera.cameraPos);

            glm::vec3 rot = mainCamera.getCameraAngles();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            sb.bind(1);



            
            glUniform3f(shaderRayTracingProgram.getUniform("camera.Position"), mainCamera.cameraPos.x, mainCamera.cameraPos.y, mainCamera.cameraPos.z);
            // glUniform3f(shaderRayTracingProgram.getUniform("camera.Forward"), mainCamera.cameraFront.x, mainCamera.cameraFront.y, mainCamera.cameraFront.z);
            glUniform3f(shaderRayTracingProgram.getUniform("camera.Forward"), rot.x, rot.y, rot.z);
            glUniformMatrix4fv(shaderRayTracingProgram.getUniform("camera.View"), 1, GL_FALSE, &mainCamera.getView()[0][0]);
            glUniformMatrix4fv(shaderRayTracingProgram.getUniform("camera.Model"), 1, GL_FALSE, &cameraModel[0][0]);
            glUniformMatrix4fv(shaderRayTracingProgram.getUniform("camera.Projection"), 1, GL_FALSE, glm::value_ptr(mainCamera.projection));

            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, VBO);
           glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, VBO2);
           glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, NBO);
            glDispatchCompute(width/64, height/64, 1);
            //glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
            glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        timingCalc(window);
        dt += 0.01;
        double now = glfwGetTime();
        glClearColor(0.5, 0.5, 0.5, 1.0f);
       

       
        testPlainShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1f(testPlainShader.getUniform("Main"), 0);
        quad.Draw(testPlainShader);
        //glDepthMask(GL_FALSE);
        //skyBoxShader.use();
        //cm.bind(0);
        //glm::mat4 view = glm::mat4(glm::mat3(mainCamera.getView()));
        //glUniformMatrix4fv(skyBoxShader.getUniform("view"), 1, GL_FALSE, &view[0][0]);
        //glUniformMatrix4fv(skyBoxShader.getUniform("projection"), 1, GL_FALSE, glm::value_ptr(mainCamera.projection));
        ////glUniform1f(skyBoxShader.getUniform("skybox"), 0);
        //skyboxCube.Draw(skyBoxShader);
        //glDepthMask(GL_TRUE);
        //glBindTexture(GL_TEXTURE_2D, texture);
        // for (size_t i = 0; i < 4; i++)
        // {
        //     shaderLightProgram.use();
        //     glm::mat4 cubeModel = glm::mat4(1.0f);
        //     cubeModel = glm::translate(cubeModel, lightCubesPos[i]);
        //     glUniformMatrix4fv(shaderLightProgram.getUniform("model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
        //     glUniformMatrix4fv(shaderLightProgram.getUniform("view"), 1, GL_FALSE, &mainCamera.getView()[0][0]);
        //     glUniformMatrix4fv(shaderLightProgram.getUniform("projection"), 1, GL_FALSE, glm::value_ptr(mainCamera.projection));
        //     glUniform1f(shaderLightProgram.getUniform("Main"), 0);
        //    // glUniform3f(shaderLightProgram.getUniform("LightSourceColor"), lightCubesColors[i].x, lightCubesColors[i].y, lightCubesColors[i].z);
        //     lightCube.Draw(shaderLightProgram);
        // }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }



}