#include <Windows.h>
#include <commdlg.h>
#include <ShlObj.h>
#include <ShObjIdl.h>

#include <string>
#include <sstream>
#include <fstream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <imgui.h>
#include <json.hpp>

#include <iostream>
namespace Utility
{
    using json = nlohmann::json;

    static std::string GetDirPath(GLFWwindow* window)
    {
        std::string resultPath = "";

        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        bool coInitialized = SUCCEEDED(hr) || hr == S_FALSE;

        IFileOpenDialog* pFileOpen = nullptr;
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            FILEOPENDIALOGOPTIONS opt;
            pFileOpen->GetOptions(&opt);
            pFileOpen->SetOptions(opt | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);

            HWND hwndOwner = glfwGetWin32Window(window);
            hr = pFileOpen->Show(hwndOwner);

            if (SUCCEEDED(hr))
            {
                IShellItem* pItem = nullptr;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFolderPath = nullptr;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);
                    if (SUCCEEDED(hr))
                    {
                        int size_needed = WideCharToMultiByte(CP_UTF8, 0, pszFolderPath, -1, NULL, 0, NULL, NULL);
                        if (size_needed > 0)
                        {
                            resultPath.resize(size_needed - 1);
                            WideCharToMultiByte(CP_UTF8, 0, pszFolderPath, -1, &resultPath[0], size_needed, NULL, NULL);
                        }
                        CoTaskMemFree(pszFolderPath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }

        if (coInitialized)
        {
            CoUninitialize();
        }

        return resultPath;
    }
    static std::string GetFilePath(GLFWwindow* window, const char* filter)
    {
        std::string resultPath = "";
        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        CHAR currentDir[256] = { 0 };
        ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
        ofn.lStructSize = sizeof(OPENFILENAMEA);
        ofn.hwndOwner = glfwGetWin32Window(window);
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        if (GetCurrentDirectoryA(256, currentDir))
            ofn.lpstrInitialDir = currentDir;
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetOpenFileNameA(&ofn) == TRUE)
        {
            resultPath = ofn.lpstrFile;
        }
        return resultPath;
    }

    static void ImGuiTextWithScale(const char* fmt, float scale = 1.0f)
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImGui::PushFont(NULL, style.FontSizeBase * scale);
        ImGui::Text(fmt);
        ImGui::PopFont();
    }

    static void ImGuiHelpMarker(const char* desc)
    {
        ImGui::TextDisabled("(?)");
        if (ImGui::BeginItemTooltip())
        {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    static json ReadJson(const std::filesystem::path& path)
    {
        json data;
        std::ifstream f(path);
        if(f.is_open())
            data = json::parse(f);
        f.close();
        return data;
    }
}