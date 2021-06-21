#include "QuadMeshPreset.h"



std::vector<float> QuadMeshPreset::vertecies = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
  // positions   // texCoords
  -1.0f,  1.0f,  0,
  -1.0f, -1.0f,  0,
   1.0f, -1.0f,  0,

  -1.0f,  1.0f,  0,
   1.0f, -1.0f,  0,
   1.0f,  1.0f, 0,
	};
std::vector<float> QuadMeshPreset::uvs = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	  // positions   // texCoords
	   0.0f, 1.0f,
	   0.0f, 0.0f,
	   1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 0.0f,
		 1.0f, 1.0f
	};


