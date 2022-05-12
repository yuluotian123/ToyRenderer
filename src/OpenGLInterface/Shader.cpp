#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "OpenGLInterface\Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

bool Shader::setup(const std::string vertexPath, const std::string fragmentPath, const std::string geometryPath)
{
	bool gShader = geometryPath != "";
	std::string shaderFolderPath = "resource/Shaders/";

	std::string vertexCode, fragmentCode, geometryCode;
	std::stringstream vShaderStream, fShaderStream, gShaderStream;
	std::ifstream vShaderFile(shaderFolderPath + vertexPath),
		fShaderFile(shaderFolderPath + fragmentPath),
		gShaderFile(shaderFolderPath + geometryPath);

	if (!vShaderFile.good()) {
		std::cout << "couldn't find VertexShader in " <<shaderFolderPath<< vertexPath  << std::endl;
		return false;
	}
	else {
		if (!fShaderFile.good()) {
			std::cout << "couldn't find FragmentShader in " << shaderFolderPath << fragmentPath  << std::endl;
			return false;
		}
		else {
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			if (gShader) { gShaderStream << gShaderFile.rdbuf(); }

			//Close Files
			vShaderFile.close();
			fShaderFile.close();
			gShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

			const char* vShaderCode = vertexCode.c_str();
			const char* fShaderCode = fragmentCode.c_str();
			const char* gShaderCode;

			if (gShader) {
				geometryCode = gShaderStream.str();
				gShaderCode = geometryCode.c_str();
			}
			//Vertex shader
			int vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vShaderCode, NULL);
			glCompileShader(vertexShader);
			int success;
			char infoLog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				std::cout << "Vertex shader compilation failed" << infoLog << std::endl;
				return false;
			}

			//Fragment shader
			int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
			glCompileShader(fragmentShader);
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				std::cout << "Fragement shader compilation failed" << infoLog << std::endl;
				return false;
			}
			//Geometry shader
			int geometryShader;
			if (gShader) {
				geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
				glShaderSource(geometryShader, 1, &gShaderCode, NULL);
				glCompileShader(geometryShader);
				glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
				if (!success) {
					glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
					std::cout << "Geometry shader compilation failed" << infoLog << std::endl;
					return false;
				}
			}

			//Linking shaders
			ID = glCreateProgram();
			glAttachShader(ID, vertexShader);
			glAttachShader(ID, fragmentShader);
			if (gShader) { glAttachShader(ID, geometryShader); }
			glLinkProgram(ID);

			glGetProgramiv(ID, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(ID, 512, NULL, infoLog);
				printf("Shader Linking failed %s\n", infoLog);
				return false;
			}

			//Deleting shaders
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			if (gShader) { glDeleteShader(geometryShader); }

			VPath = vertexPath;
			FPath = fragmentPath;
			GPath = geometryPath;

			return true;
		}
	}
}

bool Shader::setup(const std::string computePath)
{
	std::string shaderFolderPath = "resource/Shaders/";

	std::string computeCode;
	std::stringstream cShaderStream;
	std::ifstream cShaderFile(shaderFolderPath + computePath);
	if(!cShaderFile.good()) {
		std::cout << "couldn't find ComputeShader in " << shaderFolderPath << computePath << std::endl;
		return false;
	}
	else {
		cShaderStream << cShaderFile.rdbuf();

		cShaderFile.close();

		computeCode = cShaderStream.str();
		const char* cShaderCode = computeCode.c_str();

		int computeShader = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(computeShader, 1, &cShaderCode, NULL);
		glCompileShader(computeShader);
		int success;
		char infoLog[512];
		glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(computeShader, 512, NULL, infoLog);
			std::cout << "Computer shader compilation failed" << infoLog << std::endl;
			return false;
		}

		ID = glCreateProgram();
		glAttachShader(ID, computeShader);
		glLinkProgram(ID);

		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			printf("Shader Linking failed %s\n", infoLog);
			return false;
		}

		CPath = computePath;

		glDeleteShader(computeShader);
		return true;
	}
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::Use() const
{
	if (!ID) {
		printf("You cant use a Shader not set.\n");
		return;
	}
	glUseProgram(ID);
}

void Shader::Dispatch(unsigned int x, unsigned int y, unsigned int z) const
{
	if (!ID) {
		printf("You cant dispatch a Shader not set.\n");
		return;
	}
	if (CPath == "") {
		printf("Shader is not a Compute Shader.\n");
		return;
	}
	glDispatchCompute(x, y, z);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setVec3(const std::string& name, const glm::vec3& vec) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
}

void Shader::setVec4(const std::string& name, const glm::vec4& vec) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
}