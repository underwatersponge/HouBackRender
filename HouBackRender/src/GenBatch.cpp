#include "GenBatch.h"
#include "Utility.cpp"

#include <iostream>
#include <fstream>
#include <algorithm>
GenBatch::GenBatch()
{

}

uint32_t GenBatch::m_Index = 1;

// TODO:use better way
void GenBatch::WriteBatchToFile()
{
    std::string filename = "text.bat";
    filename = "test" + std::to_string(m_Index) + ".bat";
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
        std::string line0 = "set PATH=%PATH%;" + m_HouBinPath;
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
        m_HouBinPath = dirpath;
}

void GenBatch::AddHipPath(const std::string& hippath)
{
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
    m_HouBinPath = path;
}

void GenBatch::AddHipPathFromFile(GLFWwindow* window, char* filter)
{
    std::string path = Utility::GetFilePath(window, filter);
    std::replace(path.begin(), path.end(), '\\', '/');
    AddHipPath(path);
}

/*
std::string s = R"( hscript -c "mread C:/Users/HRXlubang/Desktop/TestHIP/Test.hip;
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
