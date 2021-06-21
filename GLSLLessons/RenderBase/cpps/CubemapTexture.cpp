#include "CubemapTexture.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
void CubemapTexture::loadCubemap(std::vector<std::string> texturesData)
{

   
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    int width, height, nrChannels;
	for (unsigned int i = 0; i < texturesData.size(); i++)
	{
      
      
        unsigned char* data = stbi_load(texturesData[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X +i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
	}



}

void CubemapTexture::bind(int n)
{
  //  glActiveTexture(GL_TEXTURE0 + n); // Texture unit 0
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}
