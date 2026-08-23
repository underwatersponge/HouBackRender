#include "GenBatch.h"
#include "Utility.cpp"

#include <iostream>
#include <fstream>
#include <algorithm>

GenBatch::GenBatch(std::filesystem::path jsonpath)
{
    Init(jsonpath);
}

void GenBatch::Init(std::filesystem::path jsonpath)
{
    GenFromFile(jsonpath);
}

uint32_t GenBatch::m_Index = 1;

// TODO:use better way
void GenBatch::WriteBatchFile()
{
    std::string filename = "test" + std::to_string(m_Index) + ".bat";
    std::fstream fileOut;
    
    fileOut.open(filename, std::ios_base::out);
    if (!fileOut.is_open())
    {
        std::cout <<"error have occur " << "\n";
    }
    else
    {
        if (0)
        {
            fileOut << R"(set PATH=%PATH%;C:\Program Files\Side Effects Software\Houdini 20.5.278\bin)";
            fileOut << "\n";
            fileOut << R"( hscript -c "mread C:/Users/HRXlubang/Desktop/TestHIP/Test.hip; render -V obj/Pars/CacheNode/render;render -V out/RenNode;render -V out/RenNode1;exist;exist")";
        }
        std::string line0 = "set PATH=%PATH%;" + m_HouBinDir;
        std::string line1 = SplicString();

        fileOut << line0;
        fileOut << "\n";
        fileOut << line1;
    }
    ++m_Index;
}

void GenBatch::SetHouBinPath(const std::string& dirpath)
{
    if(!dirpath.empty())
        m_HouBinDir = dirpath;
}

void GenBatch::AddHipPath(const std::string& hippath)
{
    // TODO: true multi file support
    m_HipFiles.clear();
    m_RenNodes.shrink_to_fit();
    if(!hippath.empty())
       m_HipFiles.emplace_back(hippath);
}

void GenBatch::AddHouRenNodePath(const std::string& nodepath)
{
    if(!nodepath.empty())
        m_RenNodes.emplace_back(nodepath);
}

void GenBatch::SetHouBinPathFromDir(GLFWwindow* window)
{
    std::string path = Utility::GetDirPath(window);
    m_HouBinDir = path;
}

void GenBatch::AddHipPathFromFile(GLFWwindow* window, char* filter)
{
    std::string path = Utility::GetFilePath(window, filter);
    std::replace(path.begin(), path.end(), '\\', '/');
    CleanAllRenNode();
    AddHipPath(path);
}

void GenBatch::CleanHipFiles()
{
    m_HipFiles.clear();
    m_HipFiles.shrink_to_fit();
}

void GenBatch::CleanAllRenNode()
{
    m_RenNodes.clear();
    m_RenNodes.shrink_to_fit();
#ifdef _DEBUG
    std::cout << "size:" << m_RenNodes.size() <<"storage:" << m_RenNodes.capacity() << std::endl;
#endif
}

void GenBatch::GenFromFile(const std::filesystem::path jsonpath)
{
    const json jsonData = Utility::ReadJson(jsonpath);
    //std::cout << jsonData << "\n";

    // TODO: multi hip file support error handle
    if (jsonData.contains("HouBinDir"))
    {

        m_HouBinDir = jsonData.at("HouBinDir").get<std::string>();
#ifdef _DEBUG
        std::cout << m_HouBinDir << "\n";
#endif
    }

    if (jsonData.contains("HipPath"))
    {
        std::string hipFile  = jsonData.at("HipPath").get<std::string>();
        m_HipFiles.emplace_back(hipFile);
#ifdef _DEBUG
        std::cout << m_HipFiles[0] << "\n";
#endif
    }

    if (jsonData.contains("RenNodes"))
    {
        std::vector<std::string> renNodePaths = jsonData.at("RenNodes").get<std::vector<std::string>>();
#ifdef _DEBUG
        for (auto ite = renNodePaths.begin(); ite != renNodePaths.end(); ++ite)
        {
            std::cout << *ite << "\n";
        }
#endif
        m_RenNodes = renNodePaths;
    }
}

void GenBatch::WriteCurSettingToFile(std::filesystem::path jsonpath, GenBatch::SaveSettingType type)
{
    json settings;
    switch (type) {

        case(SaveSettingType::OnlyHouBinPath):
        {
            settings = {
                {"HouBinDir", m_HouBinDir}
            };
            break;
        }
        case(SaveSettingType::BinDirAHipPath):
        {
            settings = {
                {"HouBinDir", m_HouBinDir},
                {"HipPath", m_HipFiles.at(0)},
            };
            break;
        }
        case(SaveSettingType::All):
        {
            settings = {
            {"HouBinDir", m_HouBinDir},
            {"HipPath", m_HipFiles.at(0)},
            {"RenNodes", m_RenNodes},
            };
            break;
        }
        default:
        {
            settings = {
                {"HouBinDir", m_HouBinDir}
            };
            break;
        }
    }
#ifdef _DEBUG
    std::cout << "\n";
    std::cout << settings; 
#endif
    std::string s = settings.dump(4);
    std::ofstream outFile(jsonpath.c_str(), std::ios::out);
    if (!outFile.is_open())
    {
        std::cerr << "can not open file " << "\n";
    }
    outFile << s << std::endl;
    outFile.close();
}

/*
std::string s = R"( hscript -c "mread C:/Users/UserName/Desktop/TestHIP/Test.hip;
render -V obj/Pars/CacheNode/render;render -V out/RenNode;render -V out/RenNode1;exist;exist")";
*/
// TODO: use better way
std::string GenBatch::SplicString()
{
    std::string line = R"(hscript -c "mread )" + m_HipFiles[0] + ";";
    for (auto ite = m_RenNodes.begin(); ite != m_RenNodes.end(); ++ite)
    {
        line += R"(render -V )" + *ite + ";";
    }
    line += R"(exit;exit;")";
    return line;
}
