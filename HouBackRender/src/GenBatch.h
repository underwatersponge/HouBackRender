#pragma once

#include "RenContainerNode.h"

#include <json.hpp>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

class GenBatch
{
    using json = nlohmann::json;
public:
    enum class SaveSettingType : uint32_t;
    GenBatch();
    GenBatch(std::filesystem::path configPath, std::filesystem::path renListPath = std::filesystem::path());

    void Init(const std::filesystem::path &configPath, const std::filesystem::path& renInfoPath = std::filesystem::path());
    void ReadConfigFromFile(const std::filesystem::path& jsonPath);
    void ReadRenContainerFromFile(const std::filesystem::path& jsonPath);
    
    void SetHouBinPath(const std::string& dirPath);
    void AddHipPath(const std::string& hipPath);
    void AddHouRenNodePath(size_t index, const std::string& nodePath);

    void SetHouBinPathFromDir(GLFWwindow* window);
    void AddHipPathFromFile(GLFWwindow* window, const char* filter = ".hip");

    void Clean(size_t index);
    void CleanAll();
    
    void GenRunPyBatch();
    void GenConfigJson();
    void GenRenListJson();

    size_t GetSize() const;
 
    const std::string& GetHouBinDir() const
    {
        return m_HouBinDir;
    }
    const std::string GetHipPath(uint32_t index = 0) const
    {
        if(m_RenContainer.size() >0 && index <= m_RenContainer.size())
            return m_RenContainer.at(index).GetHipFilePath();
        return std::string();
    }
    const std::vector<std::string> GetNodesPaths(uint32_t index = 0) const
    {
        if(m_RenContainer.size() > 0 && index <= m_RenContainer.size())
            return m_RenContainer.at(index).GetRenNodes();
        return std::vector<std::string>();
    }
public:
    enum class SaveSettingType : uint32_t
    {
        OnlyHouBinPath = 0,
        BinDirAHipPath = 1,
        All = 2 // do not use 
    };
private:
    std::string m_HouBinDir;
    std::vector<RenContainerNode> m_RenContainer;
};
