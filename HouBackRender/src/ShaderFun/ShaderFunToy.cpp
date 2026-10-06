#include "ShaderFunToy.h"

ShaderFunToy::ShaderFunToy()
{
}

ShaderFunToy::~ShaderFunToy()
{
}

void ShaderFunToy::Init()
{
    SetResolution();
    
    InitVAO();
    InitVBO();
    InitEBO();
    CreateAttachTexture();
    InitFramebuffer();
    AttackTexToBuffer();
}

void ShaderFunToy::InitVAO()
{
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
    }
}

void ShaderFunToy::InitVBO()
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void ShaderFunToy::InitEBO()
{
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_DYNAMIC_DRAW);
}

void ShaderFunToy::CreateAttachTexture()
{
    glGenTextures(1, &AttachColorTexture);
    glBindTexture(GL_TEXTURE_2D, AttachColorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (int)ResolutionWidth, (int)ResolutionHeight, 0, GL_RGB,GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void ShaderFunToy::ResizeAttachTexture()
{
    glBindTexture(GL_TEXTURE_2D, AttachColorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (int)ResolutionWidth, (int)ResolutionHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //AttackTexToBuffer();
}

void ShaderFunToy::InitFramebuffer()
{
    glGenFramebuffers(1, &Framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
}

void ShaderFunToy::AttackTexToBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, AttachColorTexture, 0);
}

void ShaderFunToy::Clear()
{
    glClearColor(0.f,0.f,1.f,1.f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void ShaderFunToy::ShutDown()
{
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShaderFunToy::Update()
{
    ResizeAttachTexture();
    AttackTexToBuffer();

    glViewport(0, 0, (int)ResolutionWidth, (int)ResolutionHeight);
    Clear();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    
    ShutDown();
}
