#pragma once
#include <string>
#include <sstream>

#include <glad/glad.h>

enum class ShaderType:uint32_t
{
    VertexShader = 0,
    FragmentShader = 1
};

class Shader
{
public:
    std::string Create(const char* vertexCodeSrc = nullptr, const char* fragmentCodeSrc = nullptr);
    std::string ReCreate(const char* vertexCodeSrc, const char* fragmentCodeSrc);
    void Use() const;
private:
    void CreatePragma();
    void CreateShader(const char* codeStr, ShaderType type, std::stringstream& errorSS);
    void LinkProgram(std::stringstream& errorSS);
    
private:
    uint32_t Program;
};
