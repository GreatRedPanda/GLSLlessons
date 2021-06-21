#include "DissolveExample.h"

DissolveExample::DissolveExample()
{

     //mainCamera = camera;

    shaderProgram.loadShaderFromFile("shaders/vertex.glsl", GL_VERTEX_SHADER);
    shaderProgram.loadShaderFromFile("shaders/Dissolve.glsl", GL_FRAGMENT_SHADER);
    shaderProgram.attachAndCompile();
  
    shaderLightProgram.loadShaderFromFile("shaders/vertex.glsl", GL_VERTEX_SHADER);
    shaderLightProgram.loadShaderFromFile("shaders/LightShader.glsl", GL_FRAGMENT_SHADER);
    shaderLightProgram.attachAndCompile();
}

void DissolveExample::renderLoop( Camera& mainCamera)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    dt += 0.01;
    double now = glfwGetTime();
    glClearColor(0.5, 0.5, 0.5, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
        glUniform1f(shaderProgram.getUniform("Time"), now);
        cube.Draw(shaderProgram);
    }
}
