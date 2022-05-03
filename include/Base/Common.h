#pragma once
#include <string>
constexpr  int SCREEN_WIDTH = 1980;
constexpr int SCREEN_HEIGHT = 1080;
constexpr float SCREEN_ASPECT_RATIO = SCREEN_WIDTH / (float)SCREEN_HEIGHT;

//������δ����չ��������
typedef int Shaderid;
typedef std::string Materialid;

//����Ϊ��͵��,����Ҫ��������flags������
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
