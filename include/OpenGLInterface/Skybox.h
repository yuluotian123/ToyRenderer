#pragma once
#include "OpenGLInterface\Texture.h"
#include <string>
//基本照搬的别人的实现 未来可能会进行修改
class Skybox
{
public:
	void draw();

	void setup(const std::string& path, bool isHDR, unsigned int resolution);
	void fillCubeMapWithHDR(const Shaderid buildCubeMapShader);

    unsigned& getTextureID() { return skyCube.ID; };
	const unsigned getResolution() const { return resolution; };
	const bool IsHDR() const { return ishdr; };
private:
	bool ishdr;
	unsigned int resolution;

	Texture	equirectangularMap;
	CubeMap skyCube;
};

