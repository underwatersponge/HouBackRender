#pragma once

#include <vector>
#include <string>

#include <json.hpp>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

class GenBatch
{
    using json = nlohmann::json;
    enum class SaveSettingType : uint32_t;
public:
    GenBatch(std::filesystem::path jsoonpath);
    void Init(std::filesystem::path jsonpath);

    void WriteBatchFile();

    void SetHouBinPath(const std::string& dirpath);
    void AddHipPath(const std::string& hippath);
    void AddHouRenNodePath(const std::string& nodepath);

    void SetHouBinPathFromDir(GLFWwindow* window);
    void AddHipPathFromFile(GLFWwindow* window, char* filter = ".hip");

    void CleanHipFiles();
    void CleanAllRenNode();

    void GenFromFile(std::filesystem::path jsonpath);
    void WriteCurSettingToFile(std::filesystem::path jsonpath, GenBatch::SaveSettingType = SaveSettingType::OnlyHouBinPath);

    const std::string& GetHouBinDir() const
    {
        return m_HouBinDir;
    }
    const std::vector<std::string>& GetHipPaths() const
    {
        return m_HipFiles;
    }
    const std::vector<std::string>& GetNodesPaths() const
    {
        return m_RenNodes;
    }
public:
    enum class SaveSettingType : uint32_t
    {
        OnlyHouBinPath = 0,
        BinDirAHipPath = 1,
        All = 2 // do not use 
    };
private:
    std::string SplicString();
private:
    static uint32_t m_Index;
    std::string m_HouBinDir;
    std::vector<std::string> m_HipFiles;
    std::vector<std::string> m_RenNodes;
};
