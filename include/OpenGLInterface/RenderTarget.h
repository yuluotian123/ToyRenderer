#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "OpenGLInterface\Texture.h"
#include "glm\glm.hpp"
#include <vector>
//拙劣的模仿
class RenderTarget
{
public:
	enum ENUM_TYPE 
	{
		ENUM_TYPE_INVALID,
		ENUM_TYPE_CAPTURE,
		ENUM_TYPE_BASIC,
		ENUM_TYPE_RGBF1_DEPTH,
		ENUM_TYPE_RGBF2_DEPTH,
		ENUM_TYPE_RGBF3_DEPTH,
		ENUM_TYPE_MSAA,
		ENUM_TYPE_COLOR,
		ENUM_TYPE_RED,
		ENUM_TYPE_COLOR_FLOAT,
		ENUM_TYPE_DEPTH,
		ENUM_TYPE_DEPTH_ARRAY,
		ENUM_TYPE_CUBE_DEPTH,
		ENUM_TYPE_GBUFFER,
		ENUM_TYPE_DYNAMIC_COLOR
	};

	bool createRenderTarget(unsigned width, unsigned height, ENUM_TYPE type,unsigned arrayNum = 0);

	void use() const;
	bool blitTo(const RenderTarget& FBO, GLbitfield mask);

	void clearSelf(bool inuse = true,GLbitfield clearTarget = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, glm::vec3 color = { 0.1f,0.1f,0.1f });
	static void clear(GLbitfield clearTarget = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,glm::vec3 color = {0.1f,0.1f,0.1f});//清理当前fbo

	void resizeforCapture(unsigned int width, unsigned int height);

	const Texture& getcolorTexture(unsigned int id) const;
	const Texture& getdepthTexture() const;
	unsigned int GetID() const;
	unsigned int GetRBO() const;
	bool IsValid() const { return isValid; };

	static void UseDefault();
private:
	bool isComplete();

	bool GenCapture(unsigned width, unsigned height);
	bool GenBasic(unsigned width, unsigned height);
	bool GenRGBF_Depth(unsigned width, unsigned height, unsigned colorBufferNum = 1);
	bool GenColor(unsigned width, unsigned height, bool isFloat = false);
	bool GenRed(unsigned width, unsigned height);
	bool GenDepth(unsigned width, unsigned height);
	bool GenDepthArray(unsigned width, unsigned height,unsigned depthNum);
	bool GenCubeDepth(unsigned width, unsigned height);
private:
	ENUM_TYPE type;
	unsigned int width,height;
	bool isValid = false;
	unsigned int ID;
	unsigned int RBOid;//只在GenCapture中使用

	std::vector<Texture> colorTextures;
	Texture depthTexture;
	CubeMap depthCubeTexture;//有很大优化余地
};

