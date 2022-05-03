#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "OpenGLInterface\Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "gli\gli.hpp"
#include <iostream>

void Texture::loadTexture(const std::string& filePath, bool sRGB)
{
	path = filePath;
    std::replace(path.begin(), path.end(), '\\', '/');
    std::string fileExtension = getFileExtension(filePath);
    if (fileExtension == "dds") {
        ID = loadDDSTexture(path.c_str());
    }
    else {
        stbi_set_flip_vertically_on_load(true);

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

unsigned int Texture::loadDDSTexture(const char* filePath)
{
    gli::texture Texture = gli::load(filePath);
    if (Texture.empty())
        return 0;

    gli::gl GL(gli::gl::PROFILE_GL33);
    gli::gl::format const Format = GL.translate(Texture.format(), Texture.swizzles());
    GLenum Target = GL.translate(Texture.target());

    unsigned int  TextureName = 0;
    glGenTextures(1, &TextureName);
    glBindTexture(Target, TextureName);
    glTexParameteri(Target, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(Target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(Texture.levels() - 1));
    glTexParameteri(Target, GL_TEXTURE_SWIZZLE_R, Format.Swizzles[0]);
    glTexParameteri(Target, GL_TEXTURE_SWIZZLE_G, Format.Swizzles[1]);
    glTexParameteri(Target, GL_TEXTURE_SWIZZLE_B, Format.Swizzles[2]);
    glTexParameteri(Target, GL_TEXTURE_SWIZZLE_A, Format.Swizzles[3]);

    glm::tvec3<GLsizei> const Extent(Texture.extent());
    GLsizei const FaceTotal = static_cast<GLsizei>(Texture.layers() * Texture.faces());

    switch (Texture.target())
    {
    case gli::TARGET_1D:
        glTexStorage1D(
            Target, static_cast<GLint>(Texture.levels()), Format.Internal, Extent.x);
        break;
    case gli::TARGET_1D_ARRAY:
    case gli::TARGET_2D:
    case gli::TARGET_CUBE:
        glTexStorage2D(
            Target, static_cast<GLint>(Texture.levels()), Format.Internal,
            Extent.x, Texture.target() == gli::TARGET_2D ? Extent.y : FaceTotal);
        break;
    case gli::TARGET_2D_ARRAY:
    case gli::TARGET_3D:
    case gli::TARGET_CUBE_ARRAY:
        glTexStorage3D(
            Target, static_cast<GLint>(Texture.levels()), Format.Internal,
            Extent.x, Extent.y,
            Texture.target() == gli::TARGET_3D ? Extent.z : FaceTotal);
        break;
    default:
        assert(0);
        break;
    }

    for (std::size_t Layer = 0; Layer < Texture.layers(); ++Layer)
        for (std::size_t Face = 0; Face < Texture.faces(); ++Face)
            for (std::size_t Level = 0; Level < Texture.levels(); ++Level)
            {
                GLsizei const LayerGL = static_cast<GLsizei>(Layer);
                glm::tvec3<GLsizei> Extent(Texture.extent(Level));
                Target = gli::is_target_cube(Texture.target())
                    ? static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face)
                    : Target;

                switch (Texture.target())
                {
                case gli::TARGET_1D:
                    if (gli::is_compressed(Texture.format()))
                        glCompressedTexSubImage1D(
                            Target, static_cast<GLint>(Level), 0, Extent.x,
                            Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
                            Texture.data(Layer, Face, Level));
                    else
                        glTexSubImage1D(
                            Target, static_cast<GLint>(Level), 0, Extent.x,
                            Format.External, Format.Type,
                            Texture.data(Layer, Face, Level));
                    break;
                case gli::TARGET_1D_ARRAY:
                case gli::TARGET_2D:
                case gli::TARGET_CUBE:
                    if (gli::is_compressed(Texture.format()))
                        glCompressedTexSubImage2D(
                            Target, static_cast<GLint>(Level),
                            0, 0,
                            Extent.x,
                            Texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
                            Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
                            Texture.data(Layer, Face, Level));
                    else
                        glTexSubImage2D(
                            Target, static_cast<GLint>(Level),
                            0, 0,
                            Extent.x,
                            Texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
                            Format.External, Format.Type,
                            Texture.data(Layer, Face, Level));
                    break;
                case gli::TARGET_2D_ARRAY:
                case gli::TARGET_3D:
                case gli::TARGET_CUBE_ARRAY:
                    if (gli::is_compressed(Texture.format()))
                        glCompressedTexSubImage3D(
                            Target, static_cast<GLint>(Level),
                            0, 0, 0,
                            Extent.x, Extent.y,
                            Texture.target() == gli::TARGET_3D ? Extent.z : LayerGL,
                            Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
                            Texture.data(Layer, Face, Level));
                    else
                        glTexSubImage3D(
                            Target, static_cast<GLint>(Level),
                            0, 0, 0,
                            Extent.x, Extent.y,
                            Texture.target() == gli::TARGET_3D ? Extent.z : LayerGL,
                            Format.External, Format.Type,
                            Texture.data(Layer, Face, Level));
                    break;
                default: assert(0); break;
                }
            }
    return TextureName;
}

std::string Texture::getFileExtension(const std::string& filePath)
{
    size_t indexLocation = filePath.rfind('.', filePath.length());
    if (indexLocation != std::string::npos) {
        return filePath.substr(indexLocation + 1, filePath.length() - indexLocation);
    }
    return "";
}
