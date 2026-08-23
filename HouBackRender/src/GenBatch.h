#pragma once

#include <vector>
#include <string>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

class GenBatch
{
public:
    GenBatch();
    
    void WriteBatchToFile();

    void SetHouBinPath(const std::string& dirpath);
    void AddHipPath(const std::string& hippath);
    void AddHouRenNodePath(const std::string& nodepath);

    void SetHouBinPathFromDir(GLFWwindow* window);
    void AddHipPathFromFile(GLFWwindow* window, char* filter = ".hip");

    const std::string& GetHouBinPath() const
    {
        return m_HouBinPath;
    }
    const std::vector<std::string>& GetHipPaths() const
    {
        return m_HipFiles;
    }
    const std::vector<std::string>& GetNodesPaths() const
    {
        return m_RenNodes;
    }
private:
    std::string SplicString();
private:
    static uint32_t m_Index;
    std::string m_HouBinPath;
    std::vector<std::string> m_HipFiles;
    std::vector<std::string> m_RenNodes;
};
