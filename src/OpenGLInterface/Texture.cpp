#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "OpenGLInterface\Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "gli\gli.hpp"
#include <iostream>
#include "Manager\MaterialSystem.h"
#include "OpenGLInterface\Shader.h"

Texture Texture::Invalid;

void Texture::loadTexture(const std::string& filePath, bool sRGB)
{
	path = filePath;
    std::string fileExtension = getFileExtension(filePath);
    if (fileExtension == "dds") {
        ID = loadDDSTexture(path.c_str());
    }
    else {
        stbi_set_flip_vertically_on_load(true);

        glGenTextures(1, &ID);
        GLvoid* data = stbi_load(filePath.c_str(), &width, &height, &nComponents, 0);
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

void Texture::generateTexture(const int width, const int height,const int attachnum, TEXTURETYPE type)
{
    glGenTextures(1, &ID);
    this->width = width;
    this->height = height;
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    switch (type)
    {
    case MULT_2D_HDR_COL:
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ID);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA16F, width, height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachnum, GL_TEXTURE_2D_MULTISAMPLE,ID, 0);
        typeName = "ColorBuffer";
        break;
    case SING_2D_HDR_COL:
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachnum, GL_TEXTURE_2D, ID, 0);
        typeName = "ColorBuffer";
        break;
    case SING_2D_COL:
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ID, 0);
        typeName = "ColorBuffer";
        break;
    case SING_2D_RED:
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ID, 0);
        typeName = "ColorBuffer";
        break;
    case MULT_2D_HDR_DEP:
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ID);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_DEPTH_COMPONENT32F, width, height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, ID, 0);
        typeName = "DepthBuffer";
        break;
    case SING_2D_HDR_DEP:
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ID, 0);
        typeName = "DepthBuffer";
        break;
    case SING_2D_HDR_COL_CLAMP:
        glBindTexture(GL_TEXTURE_2D,ID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachnum, GL_TEXTURE_2D, ID, 0);
        typeName = "ColorBuffer";
        break;
    case SING_2D_COL_CLAMP:
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachnum, GL_TEXTURE_2D, ID, 0);
        typeName = "ColorBuffer";
        break;
    case SING_2D_HDR_DEP_BORDER:
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ID, 0);
        typeName = "DepthBuffer";
        break;
    default:
        break;
    }
}

std::string Texture::getFileExtension(const std::string& filePath)
{
    size_t indexLocation = filePath.rfind('.', filePath.length());
    if (indexLocation != std::string::npos) {
        return filePath.substr(indexLocation + 1, filePath.length() - indexLocation);
    }
    return "";
}

void Texture::Free() {
    if (ID == 0) return;

    glDeleteTextures(1, &ID);
    typeName = "None";
    path = "";
};

const glm::mat4 CubeMap::captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
const glm::mat4 CubeMap::captureViews[18] =
{
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
};
const std::string CubeMap::fileHandleForFaces[6] = { "right.jpg", "left.jpg", "top.jpg",
                                                     "bottom.jpg", "front.jpg", "back.jpg" };//要求起名这么起
const unsigned int CubeMap::numSidesInCube = 6;

Cube CubeMap::cubeMapCube;

void CubeMap::loadCubeMap(const std::string& folderPath)
{
    stbi_set_flip_vertically_on_load(false);

    std::string path = folderPath + "/";
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

    for (unsigned int i = 0; i < numSidesInCube; ++i) {
        std::string currentFile = path + fileHandleForFaces[i];
        GLvoid* data = stbi_load(currentFile.c_str(), &width, &height, &nComponents, 0);

        if (data) {
            //Enums iteration based on OpenGL specification
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else {
            printf("Texture failed to load at path: %s \n", currentFile.c_str());
        }
        stbi_image_free(data);
    }

    //Texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    typeName = "cubemap";

    stbi_set_flip_vertically_on_load(true);
}

void CubeMap::createCubeMap(const int width, const int height, CUBEMAPTYPE cubeType)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

    switch (cubeType) {
    case  CUBEMAPTYPE::SHADOW_MAP:
        for (unsigned int i = 0; i < numSidesInCube; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_DEPTH_COMPONENT, width, height, 0,
                GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;

    case  CUBEMAPTYPE::HDR_MAP:
        for (unsigned int i = 0; i < numSidesInCube; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB32F,
                width, height, 0,
                GL_RGB, GL_FLOAT, NULL);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        break;

    case  CUBEMAPTYPE::PREFILTER_MAP:
        for (unsigned int i = 0; i < numSidesInCube; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB16F,
                width, height, 0,
                GL_RGB, GL_FLOAT, NULL);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        maxMipLevels = 5;

    }
    this->width = width;
    this->height = height;
    path = ""; //Generated so no file path
    typeName = "cubemap";
}

void CubeMap::equiRectangularToCubeMap(const unsigned int equirectangularMap, Shaderid transformShaderid)
{
    std::shared_ptr<Shader> trshader = MaterialSystem::getOrCreateInstance()->getRegisterShaderByID(transformShaderid);

    trshader->Use();
    trshader->useCamera = false;
    trshader->useLight = false;
    trshader->useIBL = false;

    trshader->setInt("equirectangularMap", 0);
    trshader->setMat4("captureProjection", captureProjection);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, equirectangularMap);

    for (unsigned int i = 0; i < numSidesInCube; i++) {
        trshader->setMat4("captureView", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, ID, 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cubeMapCube.draw();
    }


}

void CubeMap::convolveCubeMap(const unsigned int environmentMap, Shaderid convolveShaderid)
{
    std::shared_ptr<Shader> shader = MaterialSystem::getOrCreateInstance()->getRegisterShaderByID(convolveShaderid);
    shader->Use();
    shader->useCamera = false;
    shader->useLight = false;
    shader->useIBL = false;

    shader->setInt("environmentMap", 0);
    shader->setMat4("captureProjection", captureProjection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, environmentMap);

    for (unsigned int i = 0; i < numSidesInCube; i++) {
        shader->setMat4("captureView", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, ID, 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cubeMapCube.draw();
    }
}

void CubeMap::preFilterCubeMap(const unsigned int environmentMap, const unsigned int captureRBO, Shaderid filterShaderid)
{
    std::shared_ptr<Shader> shader = MaterialSystem::getOrCreateInstance()->getRegisterShaderByID(filterShaderid);

    shader->Use();
    shader->useCamera = false;
    shader->useLight = false;
    shader->useIBL = false;

    shader->setInt("environmentMap", 0);
    shader->setMat4("captureProjection", captureProjection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, environmentMap);

    for (unsigned int mip = 0; mip < maxMipLevels; ++mip) {
    
        unsigned int mipWidth = unsigned int(width * std::pow(0.5f, mip));
        unsigned int mipHeight = unsigned int(height * std::pow(0.5f, mip));

        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        shader->setFloat("roughness", roughness);
        for (unsigned int i = 0; i < numSidesInCube; ++i) {
            shader->setMat4("captureView", captureViews[i]);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                ID, mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            cubeMapCube.draw();
        }
    
    }
}
