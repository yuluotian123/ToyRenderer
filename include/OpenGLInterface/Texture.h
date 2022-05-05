#pragma once
#include "Base\Common.h"
#include "OpenGLInterface\Primitive.h"
#include <string>
#include <glm\glm.hpp>
// 我把cubemap单独分出一个类 所以2d和3d实际上已经分开了 所以所有texture都是Texture2D 所有cubemap都是Texturecubemap（但是会浪费一定空间）
class Texture
{
public:
	void loadTexture(const std::string& filePath, bool sRGB);
	void loadHDRTexture(const std::string& filePath);
	unsigned int loadDDSTexture(const char* filePath);	
	std::string getFileExtension(const std::string& filePath);
	void Free();
   
	static Texture Invalid;

	int width=0, height=0, nComponents=0;
	std::string typeName = "None", path = "";
	unsigned int ID = 0;
};

//抄的(但是好像完全可以放到texture里，我后悔了)
enum class CubeMapType:int {
	SHADOW_MAP,
	HDR_MAP
};

class CubeMap :public Texture {
public:
	void loadCubeMap(const std::string& folderPath);
	void generateCubeMap(const int width, const int height, CubeMapType cubeType);
	void equiRectangularToCubeMap(const unsigned int equirectangularMap, Shaderid transformShader);
	
	static Cube cubeMapCube;
	static const glm::mat4 captureViews[18];
	static const unsigned int numSidesInCube;
	static const glm::mat4 captureProjection;
	static const std::string fileHandleForFaces[6]; //Order from Opengl cubemap enums
};

