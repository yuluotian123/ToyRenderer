#pragma once
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Shader
{
public:
    bool setup(const std::string vertexPath, const std::string fragmentPath,
        const std::string geometryPath = "");
    bool setup(const std::string computePath);

    ~Shader();

    void Use() const;
    void Dispatch(unsigned int x, unsigned int y = 1, unsigned int z = 1) const;

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec3(const std::string& name, const glm::vec3& vec) const;
    void setVec4(const std::string& name, const glm::vec4& vec) const;

    unsigned int ID = 0;
    std::string VPath, FPath, GPath;
    std::string CPath = "";

    //TO DO：把他们放到Shaderid里
    bool isMaterial = false;
};

