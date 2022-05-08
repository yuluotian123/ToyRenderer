#pragma once
#include <string>
#include <glm\glm.hpp>

constexpr  int SCREEN_WIDTH = 1440;
constexpr int SCREEN_HEIGHT = 960;
constexpr float SCREEN_ASPECT_RATIO = SCREEN_WIDTH / (float)SCREEN_HEIGHT;

//纯粹是为了材质实例化服务的，结果导致用起来非常难受，需要转一道弯
typedef int Shaderid;
typedef std::string Materialid;

//struct shaderid //管理shader的类型：这块设计得太烂了
//{
//public:
//	enum class SHADERTYPE:int {
//		SHADER_DEFAULT,
//		SHADER_UNCAM,
//		SHADER_UNLIT,
//		SHADER_UNLIT_UNCAM,
//		SHADER_COMPUTE,
//		SHADER_NONE
//	};
//	shaderid(unsigned id) {
//		ID = id;
//		type = SHADERTYPE::SHADER_DEFAULT;
//	}
//	shaderid(unsigned id, SHADERTYPE _type) {
//		ID = id;
//		type = _type;
//	}
//	shaderid(const shaderid& id)
//	{
//		ID = id.ID;
//		type = id.type;
//	};
//public:
//	void Use() const;
//	void Dispatch(unsigned int x, unsigned int y = 1, unsigned int z = 1) const;
//
//	void setBool(const std::string& name, bool value) const;
//	void setInt(const std::string& name, int value) const;
//	void setFloat(const std::string& name, float value) const;
//	void setMat4(const std::string& name, const glm::mat4& mat) const;
//	void setVec3(const std::string& name, const glm::vec3& vec) const;
//	void setVec4(const std::string& name, const glm::vec4& vec) const;
//public:
//	SHADERTYPE type;
//
//	unsigned int ID;
//};
//
//struct materialid {
//public:
//	shaderid id;
//
//	std::string materialType;
//};

//纯粹为了偷懒,必须要事先声明flags才能用
#define TREEBOOL(variable,id)\
ImGui::PushID(id);\
ImGui::TableNextRow();\
ImGui::TableSetColumnIndex(0);\
ImGui::AlignTextToFramePadding();\
ImGui::TreeNodeEx(#variable, flags);\
ImGui::TableSetColumnIndex(1);\
ImGui::SetNextItemWidth(-FLT_MIN);\
ImGui::Checkbox("##value",&variable); \
ImGui::NextColumn();\
ImGui::PopID();\

#define TREESLIDE(variable,variableType,id,min,max)\
ImGui::PushID(id);\
ImGui::TableNextRow();\
ImGui::TableSetColumnIndex(0);\
ImGui::AlignTextToFramePadding();\
ImGui::TreeNodeEx(#variable, flags);\
ImGui::TableSetColumnIndex(1);\
ImGui::SetNextItemWidth(-FLT_MIN);\
ImGui::Slider##variableType("##value",&variable,min,max); \
ImGui::NextColumn();\
ImGui::PopID();\



#define TREEINPUT(variable,variableType,id)\
ImGui::PushID(id);\
ImGui::TableNextRow();\
ImGui::TableSetColumnIndex(0);\
ImGui::AlignTextToFramePadding();\
ImGui::TreeNodeEx(#variable, flags);\
ImGui::TableSetColumnIndex(1);\
ImGui::SetNextItemWidth(-FLT_MIN);\
ImGui::Input##variableType("##value",&variable); \
ImGui::NextColumn();\
ImGui::PopID();\

