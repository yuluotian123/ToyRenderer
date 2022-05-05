#pragma once
#include "Base\Common.h"
#include "OpenGLInterface\Primitive.h"
#include <string>
#include <glm\glm.hpp>
// �Ұ�cubemap�����ֳ�һ���� ����2d��3dʵ�����Ѿ��ֿ��� ��������texture����Texture2D ����cubemap����Texturecubemap�����ǻ��˷�һ���ռ䣩
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

//����(���Ǻ�����ȫ���Էŵ�texture��Һ����)
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

