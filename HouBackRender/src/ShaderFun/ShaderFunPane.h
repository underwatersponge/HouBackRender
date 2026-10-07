#pragma once
#include "Shader.h"
#include "ShaderFunToy.h"

#include <imgui.h>

namespace ShaderFun
{
    // shader code edit pane and view shader result
    static void ShaderEditPane(ShaderFunToy& shaderFunToy, Shader& shader, bool* p_open)
    {
        // TODO:not limit length
        static char text[1024 * 32] =
            "//this is from shadertoy default start shader!\n"
            "//see in https://www.shadertoy.com/new\n"
            "#version 400 core\n"
            "// do not change the below variable name\n"
            "out vec4 FragColor;\n"
            "in vec2 uv;\n"
            "uniform float inTime;\n"
            "uniform vec2 inMousePos;\n"
            "void main()\n"
            "{\n"
            "   float iTime = inTime;\n"
            "   vec2 mousePos = inMousePos;\n"
            "   vec2 inuv = vec2(uv.x, 1.0 - uv.y);\n"
            "   //invu.x *= 16.0f/9.0f;\n"
            "   vec3 col = 0.5 + 0.5 * cos(iTime + uv.xyx + vec3(0,2,4));\n"
            "   FragColor = vec4(col,1.f);\n"
            "}\n";

        const ImTextureID textureId = shaderFunToy.GetTextureID();
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar;
        ImGui::Begin("Hope you have fun with that!", p_open, windowFlags);

        static std::string errorStr;
        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_R))
        {
            errorStr.clear();
            errorStr += shader.ReCreate(nullptr, text);
        }

        static float editorAreaHeight = 500.f;
        // editor area
        ImGui::BeginChild("editArea", ImVec2(0, editorAreaHeight),0, windowFlags);
        ImGui::Columns(3);
        ImVec2 leftArea = ImGui::GetContentRegionAvail();
        float aspectRatio = shaderFunToy.GetAspectRatio();

        if (shaderFunToy.GetResolutionWidth() != (int)leftArea.x)
        {
            shaderFunToy.SetResolution((int)leftArea.x);
        }
        int width = (int)leftArea.x;
        int height = (int)(leftArea.x / aspectRatio);
        ImGui::Image(textureId, ImVec2((int)width, (int)height));
        static bool showInfo = false;
        if (ImGui::Button("RunInfo"))
        {
            showInfo = !showInfo;
        }

        ImGui::SameLine();
        if (showInfo)
        {
            int fps = shaderFunToy.GetFPS();
            ImGui::Text("%d fps", fps);
            ImGui::SameLine();
            ImGui::Dummy(ImVec2(30.f, ImGui::GetFrameHeight()));
            ImGui::SameLine();
            ImGui::Text("%d x %d", width,height);
        }
        //================
        // TODO:fix that:how to get the scrollY of InputTextMultiline(child window)? 
        ImGui::NextColumn();
        float rowHei = ImGui::GetTextLineHeight();
        int columnIndex = ImGui::GetColumnIndex();
        ImGui::SetColumnWidth(columnIndex,rowHei * 3.5f);
        int rows = int(editorAreaHeight / rowHei);
        if (ImGui::BeginTable("rows",1,ImGuiTableFlags_BordersH));
        {
            ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0,0));
            for (int row=0; row<rows; row++)
            {
                ImGui::TableNextRow(ImGuiTableRowFlags_None,rowHei);
                ImGui::TableSetColumnIndex(0);
                //ImGui::Text("%d", row+1);// will show when I almost fixed that
            }
            ImGui::PopStyleVar(1);
            ImGui::EndTable();
        }
        //=================
        
        ImGui::NextColumn();
        ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
        ImGui::InputTextMultiline("##codeEdit", text, IM_COUNTOF(text), ImVec2(-FLT_MIN, editorAreaHeight), flags);
        ImGui::EndChild();
        
        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::InvisibleButton("##separator", ImVec2(-FLT_MIN, 7.f));
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
        }
        if (ImGui::IsItemActive())
        {
            editorAreaHeight += ImGui::GetIO().MouseDelta.y;
        }
        ImGui::Text(errorStr.c_str());
        ImGui::End();
    }
}