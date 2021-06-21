#pragma once
#include "OpenGLHeaders.h"
#include "RenderersHandler.h"
#include "stb_image.h"
#include "SphereMesh.h"
#include "QuadMeshPreset.h"
class Outline_and_ForceField_Example: public RenderersHandler
{

	Texture depthMapTex;
	Texture colorBufferTex;
	Texture customBufferTex;


	unsigned int depthMapFBO;
	unsigned int depthMap;
	unsigned int framebuffer;
	unsigned int textureColorbuffer;
	unsigned int rbo;
	unsigned int customColorBuffer;
	unsigned int texCustomColorbuffer;
	ShaderProgram depthShader = ShaderProgram();
	ShaderProgram testPlainShader = ShaderProgram();
	ShaderProgram portalShader = ShaderProgram();
	ShaderProgram unlitShader = ShaderProgram();
	ShaderProgram shaderProg;
	ShaderProgram shaderLightProg;

	glm::vec3 cubesPositions[4] =
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

	Mesh quad = Mesh(QuadMeshPreset::vertecies, QuadMeshPreset::uvs);

	Texture mainT;
	Texture noiseT;


	SphereMesh sp = SphereMesh();


	Mesh sphere = Mesh(sp.vertecies, sp.normals, sp.uvs);
	glm::mat4 modelPortal = glm::mat4(1.0f);
	int shadowWidth = width;
	int shadowHeight = height;
public:
	Outline_and_ForceField_Example();
	void  renderLoop(Camera& mainCamera) override;
};

