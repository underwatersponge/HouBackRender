#include "Shader.h"
#include <vector>
#include <iostream>

void Shader::CreatePragma()
{
    Program = glCreateProgram(); 
}

void Shader::CreateShader(const char* codeStr, ShaderType type, std::stringstream& errorSS)
{
    GLenum shaderType = (type == ShaderType::VertexShader ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
    GLuint shader = glCreateShader(shaderType);

    if(codeStr != nullptr)
        glShaderSource(shader, 1, &codeStr,0);
    else
    {
        if (shaderType == GL_VERTEX_SHADER)
        {
            const char* vertexShaderSource = "#version 400 core\n"
                "layout (location = 0) in vec3 inPos;\n"
                "layout (location = 1) in vec2 inUV;\n"
                "out vec2 uv;\n"
                "void main()\n"
                "{\n"
                "uv = inUV;\n"
                "gl_Position = vec4(inPos, 1.0);\n"
                "}\n";
            glShaderSource(shader, 1, &vertexShaderSource, 0);
        }
        if (shaderType == GL_FRAGMENT_SHADER)
        {
            const char* fragmentShaderSource = "#version 400 core\n"
                "out vec4 FragColor;\n"
                "in vec2 uv;\n"
                "void main()\n"
                "{\n"
                "FragColor = vec4(uv.x,uv.y,0.f,1.f);\n"
                "}\n";

            glShaderSource(shader, 1, &fragmentShaderSource, 0);
        }
    }
    glCompileShader(shader);
    
    GLint bCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &bCompiled);
    if (bCompiled == GL_FALSE)
    {
        errorSS << "Shader compile failed:" << (shaderType == GL_VERTEX_SHADER ? "VERTEXSHADER" : "FRAGMENTSHADER") << "\n";
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        
        if (maxLength > 0)
        {
            std::vector<char> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, nullptr, infoLog.data());
            
            glDeleteShader(shader);

            errorSS << infoLog.data() << "\n";
        }
    }
    glAttachShader(Program, shader);
    glDeleteShader(shader);   
}

void Shader::LinkProgram(std::stringstream& errorSS)
{
    glLinkProgram(Program);
    
    GLint bLinked = 0;
    glGetProgramiv(Program, GL_LINK_STATUS, &bLinked);
    if (bLinked == GL_FALSE)
    {
        errorSS << "Program linking failed" << "\n";
        GLint maxLength = 0;
        glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &maxLength);
        if (maxLength > 0)
        {
            std::vector<char>  infoLog(maxLength);
            glGetProgramInfoLog(Program, maxLength, nullptr, infoLog.data());
            
            glDeleteProgram(Program);

            errorSS << infoLog.data() << "\n";
        }
    }
}

std::string Shader::Create(const char* vertexCodeSrc, const char* fragmentCodeSrc)
{
    std::stringstream errorSS;
    CreatePragma();
    CreateShader(vertexCodeSrc, ShaderType::VertexShader, errorSS);
    CreateShader(fragmentCodeSrc, ShaderType::FragmentShader, errorSS);
    LinkProgram(errorSS);

    return errorSS.str();
}

std::string Shader::ReCreate(const char* vertexCodeSrc, const char* fragmentCodeSrc)
{
    glDeleteProgram(Program);
    return(Create(vertexCodeSrc, fragmentCodeSrc));
}

void Shader::Use() const
{
    glUseProgram(Program);
}
