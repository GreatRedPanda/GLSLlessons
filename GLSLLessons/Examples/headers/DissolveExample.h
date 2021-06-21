#pragma once
#include "OpenGLHeaders.h"
#include "RenderersHandler.h"
class DissolveExample: public RenderersHandler
{

private:
   // Camera mainCamera;
	ShaderProgram shaderProgram;
	ShaderProgram shaderLightProgram;
	Mesh     cube = Mesh(CubeMeshPreset::vertecies, CubeMeshPreset::normals, CubeMeshPreset::uvs);
    Mesh lightCube = Mesh(CubeMeshPreset::vertecies, CubeMeshPreset::normals, CubeMeshPreset::uvs);
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


public: DissolveExample();
 void  renderLoop( Camera& mainCamera) override;
};

