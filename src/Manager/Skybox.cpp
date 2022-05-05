#include "glad\glad.h"
#include "OpenGLInterface\Skybox.h"

void Skybox::draw()
{
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyCube.ID);
	CubeMap::cubeMapCube.draw();
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

void Skybox::setup(const std::string& path, bool isHDR, unsigned int res)
{
    ishdr = isHDR;
    resolution = res;

    if (isHDR) {
        equirectangularMap.loadHDRTexture(path);
        skyCube.generateCubeMap(res, res, CubeMapType::HDR_MAP);
    }
    else {
        skyCube.loadCubeMap(path);//需要json传入folder路径
    }
}

void Skybox::fillCubeMapWithHDR(const Shaderid buildCubeMapShader) {
    skyCube.equiRectangularToCubeMap(equirectangularMap.ID, buildCubeMapShader);
}