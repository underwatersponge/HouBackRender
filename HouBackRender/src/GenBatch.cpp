#include "GenBatch.h"
#include "Utility.cpp"

#include <iostream>
#include <fstream>
#include <algorithm>

GenBatch::GenBatch()
{
}

GenBatch::GenBatch(std::filesystem::path configPath, std::filesystem::path renListPath)
{
    Init(configPath, renListPath);
}

void GenBatch::Init(const std::filesystem::path& configPath, const std::filesystem::path& renInfoPath)
{
    if (!configPath.empty())
        ReadConfigFromFile(configPath);
    if (!renInfoPath.empty())
        ReadRenContainerFromFile(renInfoPath);
}

void GenBatch::ReadConfigFromFile(const std::filesystem::path& jsonPath)
{
    const json jsonData = Utility::ReadJson(jsonPath);
    if (jsonData.contains("HouBinDir"))
        m_HouBinDir = jsonData.at("HouBinDir").get<std::string>();
}
void GenBatch::ReadRenContainerFromFile(const std::filesystem::path& jsonPath)
{
    const json jsonDatat = Utility::ReadJson(jsonPath);
    if (jsonDatat.contains("RenList"))
    {
        const auto& RenList = jsonDatat.at("RenList");
        for (auto ite = RenList.begin(); ite != RenList.end(); ++ite)
        {
            if (ite->contains("HipPath"))
            {
                std::string hipPath = ite->at("HipPath").get<std::string>();
                std::vector<std::string> renNodes;
                if (ite->contains("RenNode"))
                {
                    renNodes = ite->at("RenNode").get<std::vector<std::string>>();
                }
                m_RenContainer.push_back(RenContainerNode(hipPath, renNodes));
            }
        }
    }
}

void GenBatch::GenConfigJson()
{
    std::string filename = "config.json";
    json config;
    config = {
        {"HouBinDir", m_HouBinDir}
    };

    std::string s = config.dump(4);
    std::fstream fileOut;
    fileOut.open(filename, std::ios_base::out);
    if (!fileOut.is_open())
    {
        std::cout << "error can ont open file:" << filename << std::endl;
    }
    else {
        fileOut << s << std::endl;
        fileOut.close();
    }
}

void GenBatch::GenRenListJson()
{
    // test code//
    if (0) {
        std::vector<std::string> t = { "/out/a","/out/b","/out/c" };
        m_RenContainer.push_back(RenContainerNode("a.hip", t));
        m_RenContainer.push_back(RenContainerNode("b.hip", t));
    }
    std::string filename = "RenList.json";
    json renList;
    for (auto ite = m_RenContainer.begin(); ite != m_RenContainer.end(); ++ite)
    {
        const std::string& hipPath = ite->GetHipFilePath();
        const std::vector<std::string> renNodes = ite->GetRenNodes();
        
        renList["RenList"].push_back(
            { 
                {"HipPath",hipPath},
                {"RenNode", renNodes}
            }
            );
    }
    std::cout << renList;
    std::ofstream outFile(filename);
    if (!outFile.is_open())
        std::cout << "Error can not open file:" << filename;
    //std::string s = renList.dump(4);
    //outFile << s;
    outFile << std::setw(4) << renList << "\n";
    outFile.close();
}

size_t GenBatch::GetSize() const
{
    return m_RenContainer.size();
}

void GenBatch::GenRunPyBatch()
{
    std::string filename = "RunPyTest.bat";
    std::fstream fileOut;
    fileOut.open(filename, std::ios_base::out);
    if (!fileOut.is_open())
    {
        std::cout << "error can not open file:" << filename << "\n";
    }
    else
    {
        std::string hythonPath;
        const json jsonData = Utility::ReadJson("config.json");
        if (jsonData.contains("HouBinDir"))
        {
            hythonPath = jsonData.at("HouBinDir").get<std::string>() + "\\hython.exe";
            std::cout << hythonPath;
        }
        else 
        {
            if(!m_HouBinDir.empty())
                hythonPath = m_HouBinDir + "\\hython.exe";
        }
        std::string batchLine = std::string("call") + " \"" + hythonPath + "\" " + "testRen.py";// TODO:find a better method
        fileOut << batchLine << "\n";
        fileOut << "pause";
        fileOut.close();
    }
}

void GenBatch::SetHouBinPath(const std::string& dirPath)
{
    if(!dirPath.empty())
        m_HouBinDir = dirPath;
}

void GenBatch::AddHipPath(const std::string& hipPath)
{
    // TODO: true multi file support
    if(!hipPath.empty())
        m_RenContainer.emplace_back(hipPath);
}

void GenBatch::AddHouRenNodePath(size_t index, const std::string& nodePath)
{
    if (!nodePath.empty())
        m_RenContainer.at(index).AddRenNode(nodePath);
}

void GenBatch::SetHouBinPathFromDir(GLFWwindow* window)
{
    std::string path = Utility::GetDirPath(window);
    if (!path.empty())
        m_HouBinDir = path;
}

void GenBatch::AddHipPathFromFile(GLFWwindow* window, const char* filter)
{
    std::string path = Utility::GetFilePath(window, filter);
    std::replace(path.begin(), path.end(), '\\', '/');
    AddHipPath(path);
}

void GenBatch::Clean(size_t index)
{
    if (index > m_RenContainer.size())
        return;
    m_RenContainer.erase(m_RenContainer.begin() + index);
}

void GenBatch::CleanAll()
{
    m_RenContainer.clear();
}
