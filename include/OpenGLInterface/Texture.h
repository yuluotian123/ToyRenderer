#pragma once
#include "Base\Common.h"
#include <string>
//Need to Support more types of Texture
class Texture
{
public:
	void loadTexture(const std::string& filePath, bool sRGB);
	void loadHDRTexture(const std::string& filePath);
	unsigned loadDDSTexture(const char* filePath);

	std::string getFileExtension(const std::string& filePath);

	int width=0, height=0, nComponents=0;

	std::string uniformName;
	std::string path;

	unsigned int ID = 0;
};

