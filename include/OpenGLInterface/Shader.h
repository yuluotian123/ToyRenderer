#pragma once
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Shader
{
public:
    bool setup(const std::string vertexPath, const std::string fragmentPath,
        const std::string geometryPath = "");

    ~Shader();

    void Use();

    //使用material中的同名函数可能会比较好？
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec3(const std::string& name, const glm::vec3& vec) const;
    void setVec4(const std::string& name, const glm::vec4& vec) const;

    unsigned int ID = 0;
    std::string VPath, FPath, GPath;
};

