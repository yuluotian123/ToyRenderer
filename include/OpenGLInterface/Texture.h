#pragma once
#include "Base\Common.h"
#include "OpenGLInterface\Primitive.h"
#include <string>
#include <glm\glm.hpp>
// 我把cubemap单独分出一个类 所以2d和3d实际上已经分开了 所以所有texture都是Texture2D 所有cubemap都是Texturecubemap（但是会浪费一定空间）
enum TEXTURETYPE {
	MULT_2D_HDR_COL,
	SING_2D_HDR_COL,
	SING_2D_COL,
	SING_2D_RED,
	MULT_2D_HDR_DEP,
	SING_2D_HDR_DEP,
	SING_2D_DEP,
	SING_2D_HDR_COL_CLAMP,
	SING_2D_COL_CLAMP,
	SING_2D_HDR_DEP_BORDER
};
class Texture
{
public:
	void loadTexture(const std::string& filePath, bool sRGB);
	void loadHDRTexture(const std::string& filePath);
	unsigned int loadDDSTexture(const char* filePath);	
	void generateTexture(const int width, const int height, const int attachnum, TEXTURETYPE type);
	std::string getFileExtension(const std::string& filePath);
	void Free();
   
	static Texture Invalid;

	int width=0, height=0, nComponents=0;
	std::string typeName = "None", path = "";
	unsigned int ID = 0;
};

//抄的(但是好像完全可以放到texture里，我后悔了)
enum class CUBEMAPTYPE:int {
	SHADOW_MAP,
	HDR_MAP,
	PREFILTER_MAP
};

class CubeMap :public Texture {
public:
	void loadCubeMap(const std::string& folderPath);
	void createCubeMap(const int width, const int height, CUBEMAPTYPE cubeType);
	void equiRectangularToCubeMap(const unsigned int equirectangularMap, Shaderid transformShader);

	void convolveCubeMap(const unsigned int environmentMap, Shaderid convolveShader);
	void preFilterCubeMap(const unsigned int environmentMap, const unsigned int captureRBO, Shaderid filterShader);
	
	unsigned maxMipLevels;

	static Cube cubeMapCube;
	static const glm::mat4 captureViews[18];
	static const unsigned int numSidesInCube;
	static const glm::mat4 captureProjection;
	static const std::string fileHandleForFaces[6]; //Order from Opengl cubemap enums
};

