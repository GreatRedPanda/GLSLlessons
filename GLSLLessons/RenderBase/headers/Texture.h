#pragma once
//#include <fstream> 
//#include <istream> 
//#include <iostream>
#include <string>

//#include <cstdlib>
class Texture
{
protected:
	unsigned int ID;
public:

	Texture(std::string filePath);

	Texture();
	unsigned int getTextureBinding() { return this->ID; }
	Texture(int w, int h, int format, int wrapMode, int filterMode, int type);
	void virtual bind(int n);
	void loadTexture(std::string filePath);
};

