#include "SphereMesh.h"

SphereMesh::SphereMesh()
{
	float dt = 3.14 / 20;
	float theta = 2*dt;
	for (size_t i = 0; i < 20-1; i++)
	{
	
		float dtRing = 6.28 / 20;
		float phi = 0;
		for (size_t j = 0; j < 20; j++)
		{
			
			//triangles
			//1

			glm::vec3  p1;
			glm::vec3  p2;
			glm::vec3  p3;
			glm::vec3  p4;

			p1.x = sin(theta) * cos(phi);
			p1.y = sin(theta) * sin(phi);
			p1.z = cos(theta);

			p2.x = sin(theta-dt) * cos(phi);
			p2.y = sin(theta-dt) * sin(phi);
			p2.z = cos(theta-dt);
			phi += dtRing;

			p3.x = sin(theta) * cos(phi);
			p3.y = sin(theta) * sin(phi);
			p3.z = cos(theta);

			p4.x = sin(theta - dt) * cos(phi);
			p4.y = sin(theta - dt) * sin(phi);
			p4.z = cos(theta - dt);


			glm::vec3 np1=	-glm::cross(p2 - p1, p3 - p1);
			glm::vec3 np2 = glm::cross(p2- p1, p2 - p4);
			glm::vec3 np3 = -glm::cross(p1 - p3, p4 - p3);
			glm::vec3 np4 = -glm::cross(p3 - p4, p2 - p4);


			vertecies.push_back(p1.x);
			vertecies.push_back(p1.y);
			vertecies.push_back(p1.z);

			normals.push_back(np1.x);
			normals.push_back(np1.y);
			normals.push_back(np1.z);

			uvs.push_back(0);
			uvs.push_back(0);

			vertecies.push_back(p2.x);
			vertecies.push_back(p2.y);
			vertecies.push_back(p2.z);
			uvs.push_back(0);
			uvs.push_back(1);

			normals.push_back(np2.x);
			normals.push_back(np2.y);
			normals.push_back(np2.z);

			vertecies.push_back(p3.x);
			vertecies.push_back(p3.y);
			vertecies.push_back(p3.z);
			uvs.push_back(1);
			uvs.push_back(0);

			normals.push_back(np3.x);
			normals.push_back(np3.y);
			normals.push_back(np3.z);






			vertecies.push_back(p3.x);
			vertecies.push_back(p3.y);
			vertecies.push_back(p3.z);
			normals.push_back(np3.x);
			normals.push_back(np3.y);
			normals.push_back(np3.z);
			uvs.push_back(1);
			uvs.push_back(0);

			vertecies.push_back(p2.x);
			vertecies.push_back(p2.y);
			vertecies.push_back(p2.z);
			uvs.push_back(0);
			uvs.push_back(1);
			normals.push_back(np2.x);
			normals.push_back(np2.y);
			normals.push_back(np2.z);
			vertecies.push_back(p4.x);
			vertecies.push_back(p4.y);
			vertecies.push_back(p4.z);

			normals.push_back(np4.x);
			normals.push_back(np4.y);
			normals.push_back(np4.z);
			uvs.push_back(1);
			uvs.push_back(1);
		}
		theta += dt;
	}
}
