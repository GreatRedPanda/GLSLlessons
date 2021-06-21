#pragma once
# include <vector>
#include "ShaderProgram.h"
class Mesh
{
private:
	unsigned int VAO=0;
	unsigned int VBO=0;
	unsigned int NBO=0;
	unsigned int TextureBO = 0;
	int countOfTriangles;
	//float vertecies[];

public:
	Mesh();
	Mesh(const std::vector<float> &vertecies, const std::vector<float> &normals, const std::vector<float> &uvs );
	Mesh(const std::vector<float>& vertecies,  const std::vector<float>& uvs);
	Mesh(const std::vector<float>& vertecies);

	void Draw(ShaderProgram &shader);
};

