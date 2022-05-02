#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "OpenGLInterface\Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

void Texture::loadTexture(const std::string& filePath, bool sRGB)
{
    stbi_set_flip_vertically_on_load(true);

	path = filePath;
	glGenTextures(1, &ID);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nComponents, 0);
	if (data) {
		GLenum format = GL_RGBA;
		GLenum internalFormat = GL_RGBA;
        if (nComponents == 1) {
            format = GL_RED;
            internalFormat = GL_RED;
        }
        else if (nComponents == 3) {
            format = GL_RGB;
            if (sRGB) {
                internalFormat = GL_SRGB;
            }
            else {
                internalFormat = GL_RGB;
            }
        }
        else if (nComponents == 4) {
            format = GL_RGBA;
            if (sRGB) {
                internalFormat = GL_SRGB_ALPHA;
            }
            else {
                internalFormat = GL_RGBA;
            }
        }

        glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        //MipMapped and Repeating
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
		std::cout << "Fail to load at Path:" << path << std::endl;

    stbi_image_free(data);
}

void Texture::loadHDRTexture(const std::string& filePath)
{
    stbi_set_flip_vertically_on_load(true);

    float* data = stbi_loadf(filePath.c_str(), &width, &height, &nComponents, 0);
    if (data) {
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D,ID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }
    else
        std::cout << "Fail to load at Path:" << path << std::endl;

    stbi_image_free(data);
}
