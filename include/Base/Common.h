#pragma once
#include <string>
#include <glm\glm.hpp>
constexpr  int SCREEN_WIDTH = 1980;
constexpr int SCREEN_HEIGHT = 1080;
constexpr float SCREEN_ASPECT_RATIO = SCREEN_WIDTH / (float)SCREEN_HEIGHT;

//TO DO:把这两个拓展成两个类
typedef int Shaderid;
typedef std::string Materialid;

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

struct shaderid //管理shader的类型：但我没想好有哪些类型，所以现在shaderid还是typedef
{
public:
	bool useCamera = true;
	bool useLight = true;
	bool useModel = true;
	bool useIBL = true;

	unsigned int id = 0;
};