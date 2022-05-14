#include "OpenGLInterface\RenderTarget.h"
#define DEFAULT_FRAMEBUFFER 0

bool RenderTarget::createRenderTarget(unsigned _width, unsigned _height, ENUM_TYPE _type,unsigned arrayNum)
{
    width = _width;
    height = _height;
    type = _type;
    switch (type)
    {
    case RenderTarget::ENUM_TYPE_INVALID:
        printf("Invalid.\n");
        isValid = false;
        break;
    case RenderTarget::ENUM_TYPE_CAPTURE:
        if (!GenCapture(width, height))
            printf("fail to GenCapture.\n");
        break;
    case RenderTarget::ENUM_TYPE_BASIC:
        if (!GenBasic(width,height))
            printf("fail to GenBasic.\n");
        break;
    case RenderTarget::ENUM_TYPE_RGBF1_DEPTH:
    case RenderTarget::ENUM_TYPE_RGBF2_DEPTH:
    case RenderTarget::ENUM_TYPE_RGBF3_DEPTH:
    {
        const int colorBufferNum = type - ENUM_TYPE_RGBF1_DEPTH + 1;
        if (!GenRGBF_Depth(width, height, colorBufferNum))
            printf("fail ot GenRGBF_Depth.\n");
        break;
    }
    case RenderTarget::ENUM_TYPE_MSAA:
        break;
    case RenderTarget::ENUM_TYPE_COLOR:
    case RenderTarget::ENUM_TYPE_COLOR_FLOAT:
    {
        const bool isFloat = type == ENUM_TYPE_COLOR_FLOAT;
        if (!GenColor(width, height, isFloat))
            printf("fail to GenColor.\n");
        break;
    }
    case RenderTarget::ENUM_TYPE_RED:
        if (!GenRed(width, height))
            printf("fail to GenRed.\n");
        break;
    case RenderTarget::ENUM_TYPE_DEPTH:
        if (!GenDepth(width, height))
            printf("fail to GenDepth.\n");
        break;
    case RenderTarget::ENUM_TYPE_DEPTH_ARRAY:
        if (!GenDepthArray(width, height,arrayNum))
            printf("fail to GenDepth.\n");
        break;
    case RenderTarget::ENUM_TYPE_CUBE_DEPTH:
        if (!GenCubeDepth(width, height))
            printf("fail to GenCubeDepth.\n");
        break;
    case RenderTarget::ENUM_TYPE_GBUFFER:
        printf(" Require farther develop.\n");
        isValid = false;
        break;
    case RenderTarget::ENUM_TYPE_DYNAMIC_COLOR:
        printf(" Require farther develop.\n");
        isValid = false;
        break;
    default:
        printf(" wrong type of rendertarget.\n");
        isValid = false;
        break;
    }

    return isValid;
}

void RenderTarget::use() const
{
    if (!isValid) 
    { 
        printf("fail to use this rendertarget.\n"); 
        return;
    }
    glViewport(0,0,width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

bool RenderTarget::blitTo(const RenderTarget& fbo, GLbitfield mask)
{
    if (!isValid||!fbo.isValid) return false;

    glBindFramebuffer(GL_READ_FRAMEBUFFER, ID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo.ID);

    glBlitFramebuffer(0, 0, fbo.width, fbo.height, 0, 0, fbo.width, fbo.height, mask, GL_NEAREST);
    return true;
}

void RenderTarget::clear(GLbitfield clearTarget, glm::vec3 color,bool inuse)
{
    if (!isValid) return;

    glBindFramebuffer(GL_READ_FRAMEBUFFER, ID);
    glClearColor(color[0], color[1], color[2], 1.0f);
    glClear(clearTarget);
    if (!inuse)
        UseDefault();
}

void RenderTarget::clear(glm::vec3 color, GLbitfield clearTarget)
{
    glClearColor(color[0], color[1], color[2], 1.0f);
    glClear(clearTarget);
}

void RenderTarget::resizeforCapture(unsigned int width, unsigned int height)
{
    if (!isValid) return;
    if (type != ENUM_TYPE_CAPTURE) return;

    this->width = width;
    this->height = height;

    glBindRenderbuffer(GL_RENDERBUFFER, RBOid);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glViewport(0, 0, width, height);
}

const Texture& RenderTarget::getcolorTexture(unsigned int id) const
{
        if (!isValid || id > colorTextures.size())
            return Texture::Invalid;

        if (type != ENUM_TYPE_BASIC
            && type != ENUM_TYPE_MSAA
            && type != ENUM_TYPE_COLOR
            && type != ENUM_TYPE_RED
            && type != ENUM_TYPE_COLOR_FLOAT
            && type != ENUM_TYPE_RGBF1_DEPTH
            && type != ENUM_TYPE_RGBF2_DEPTH
            && type != ENUM_TYPE_RGBF3_DEPTH
            && type != ENUM_TYPE_GBUFFER)
            return Texture::Invalid;

        return colorTextures[id];
}

const Texture& RenderTarget::getdepthTexture() const
{
    if (!isValid)
        return Texture::Invalid;

    if (type != ENUM_TYPE_DEPTH
        &&type != ENUM_TYPE_DEPTH_ARRAY
        && type != ENUM_TYPE_CUBE_DEPTH
        && type != ENUM_TYPE_GBUFFER)
        return Texture::Invalid;

    return depthTexture;
}

unsigned int RenderTarget::GetID() const
{
    if (!isValid)
        return 0;

    return ID;
}

unsigned int RenderTarget::GetRBO() const
{
    if (!isValid)
        return 0;
    if (type != ENUM_TYPE_CAPTURE)
        return 0;

    return RBOid;
}

void RenderTarget::UseDefault()
{
    glViewport(0, 0,SCREEN_WIDTH, SCREEN_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, DEFAULT_FRAMEBUFFER);
}

bool RenderTarget::isComplete()
{
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf(" Failed to initialize the offscreen frame buffer!\n");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

bool RenderTarget::GenCapture(unsigned width, unsigned height)
{
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);

    unsigned int RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    isValid = isComplete();
    if (!isValid) {
        printf("Framebuffer is not complete!\n");
        return false;
    }

    RBOid = RBO;
    return true;
}

bool RenderTarget::GenBasic(unsigned width, unsigned height)
{
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);

    Texture colortexture;
    colortexture.generateTexture(width, height, 0, TEXTURETYPE::SING_2D_COL);

    unsigned int RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    isValid = isComplete();
    if (!isValid) {
        printf("Framebuffer is not complete!\n");
        return false;
    }

    colorTextures.push_back(colortexture);
    return true;
}

bool RenderTarget::GenRGBF_Depth(unsigned width, unsigned height, unsigned colorBufferNum)
{
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);

    for (unsigned i = 0; i < colorBufferNum; i++) {
        Texture texture;
        texture.generateTexture(width,height,i,TEXTURETYPE::SING_2D_HDR_COL_CLAMP);

        colorTextures.push_back(texture);
    }

    if (colorBufferNum > 1) {
        unsigned* attachments = new unsigned[colorBufferNum];
        for (unsigned i = 0; i < colorBufferNum; i++)
            attachments[i] = GL_COLOR_ATTACHMENT0 + i;
        glDrawBuffers(colorBufferNum, attachments);
        delete[] attachments;
    }

    unsigned RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    isValid = isComplete();
    if (!isValid) {
        printf("Framebuffer is not complete!\n");
        colorTextures.clear();
        return false;
    }

    return true;
}

bool RenderTarget::GenColor(unsigned width, unsigned height, bool isFloat)
{
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    // create a color attachment texture
    Texture texture;
    if (isFloat) texture.generateTexture(width, height, 0, TEXTURETYPE::SING_2D_HDR_COL_CLAMP);
    else texture.generateTexture(width, height, 0, TEXTURETYPE::SING_2D_COL_CLAMP);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    isValid = isComplete();
    if (!isValid) {
        printf("Framebuffer is not complete!\n");
        return false;
    }

    colorTextures.push_back(texture);
    return true;
}

bool RenderTarget::GenRed(unsigned width, unsigned height)
{
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    // create a color attachment texture
    Texture texture;
    texture.generateTexture(width,height,0,TEXTURETYPE::SING_2D_RED);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    isValid = isComplete();
    if (!isValid) {
        printf("Framebuffer is not complete!\n");
        return false;
    }

    colorTextures.push_back(texture);
    return true;
}

bool RenderTarget::GenDepth(unsigned width, unsigned height)
{
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    // create depth texture
    depthTexture.generateTexture(width, height, 0, TEXTURETYPE::SING_2D_HDR_DEP_BORDER);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    isValid = isComplete();
    if (!isValid) {
        printf("Framebuffer is not complete!\n");
        return false;
    }

    return true;
}

bool RenderTarget::GenDepthArray(unsigned width, unsigned height,unsigned depthNum)
{
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    depthTexture.generateTexture(width, height, depthNum, TEXTURETYPE::SING_2D_ARRAY_HDR_DEP_BORDER);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    isValid = isComplete();
    if (!isValid) {
        printf("Framebuffer is not complete!\n");
        return false;
    }
}

bool RenderTarget::GenCubeDepth(unsigned width, unsigned height)
{
    glGenFramebuffers(1, &ID);
    
    depthCubeTexture.createCubeMap(width, height, CUBEMAPTYPE::SHADOW_MAP);

    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeTexture.ID, 0);
    glDrawBuffer(GL_NONE);  
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    isValid = isComplete();
    if (!isValid) {
        printf("Framebuffer is not complete!\n");
        return false;
    }

    return true;
}

