#pragma once
#include <glad/glad.h>

class ShaderFunToy
{
public:
    ShaderFunToy();
    ~ShaderFunToy();
    void Init();

    void SetResolution(unsigned int width = 800)
    {
        ResolutionWidth = width;
        ResolutionHeight = (float)width/AspectRatio;
    }
    void Update();
    void ShutDown();
    
    GLuint GetTextureID()const{return AttachColorTexture;}
    unsigned int GetResolutionWidth() const { return ResolutionWidth; }
    unsigned int GetResolutionHeight() const { return ResolutionHeight; }
    float GetAspectRatio() const {return (float)ResolutionWidth / (float)ResolutionHeight; }
private:
    void InitVAO();
    void InitVBO();
    void InitEBO();
    void CreateAttachTexture();
    void ResizeAttachTexture();
    void InitFramebuffer();
    void AttackTexToBuffer();
    
    void Clear();
private:
    const float Vertices[20] = {
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f
    };
    const int Indices[6] = {
        0,1,3,
        1,2,3
    };
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int Framebuffer;
    unsigned int AttachColorTexture;
    
    float AspectRatio = 16.0f / 9.f;
    unsigned int ResolutionWidth= 1;
    unsigned int ResolutionHeight= 1;
};
