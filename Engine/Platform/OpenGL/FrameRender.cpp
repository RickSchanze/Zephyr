/**
 * @file FrameRender.cpp
 * @author Echo
 * @Date 24-3-3
 * @brief
 */

#include "FrameRender.h"

#include <iostream>

namespace Platform::GL
{

auto vertexShaderSource = "#version 330 core\n"
                          "layout (location = 0) in vec3 aPos;\n"
                          "void main()\n"
                          "{\n"
                          "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                          "}\0";
auto fragmentShaderSource = "#version 330 core\n"
                            "out vec4 FragColor;\n"
                            "void main()\n"
                            "{\n"
                            "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                            "}\n\0";

float vertices[] = {
    0.5f, 0.5f, 0.0f,   // top right
    0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f   // top left
};
unsigned int indices[] = {
    // note that we start from 0!
    0, 1, 3, // first Triangle
    1, 2, 3  // second Triangle
};

bool FrameRender::Initialize(const int width, const int height)
{
    // 创建各个需要的对象
    m_render_texture = std::make_unique<Texture>();
    m_rbo = std::make_unique<RenderBufferObject>();
    m_fbo = std::make_unique<FrameBufferObject>();

    // 配置各个对象
    // 配置texture
    m_render_texture->Initialize(width, height);
    // 配置rbo
    m_rbo->Bind();
    m_rbo->AllocateStorage(width, height);
    // 配置fbo
    m_fbo->Bind();
    m_fbo->AttachTexture(*m_render_texture);
    m_fbo->AttachRenderBuffer(*m_rbo);

    // TODO: 删除vao和vbo
    m_vao = std::make_unique<VertexArrayObject>();
    m_vbo = std::make_unique<VertexBufferObject>();
    m_ebo = std::make_unique<ElementBufferObject>();
    m_vao->Bind();
    m_vbo->Bind();
    m_vbo->SetData(vertices, sizeof(vertices), GL_STATIC_DRAW, GL_ARRAY_BUFFER);
    m_vbo->BindVertexAttributePointer(3, GL_FLOAT);
    m_ebo->Bind();
    m_ebo->SetData(indices, sizeof(indices));

    // -----------------DELETE ME------------------------------
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    m_shader = glCreateProgram();
    glAttachShader(m_shader, vertexShader);
    glAttachShader(m_shader, fragmentShader);
    glLinkProgram(m_shader);
    //-----------------------DELETE ME----------------------------------------
    return m_fbo->Check();
}

void FrameRender::Draw() const
{
    m_fbo->Bind();
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glUseProgram(m_shader);
    m_vao->Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
    m_fbo->Unbind();
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT);
}

void FrameRender::Resize(const int width, const int height) const
{
    m_render_texture->Bind();
    m_render_texture->SetImageParam({.width = width, .height = height});
    m_fbo->AttachTexture(*m_render_texture);

    m_rbo->Bind();
    m_rbo->AllocateStorage(width, height);
    m_fbo->AttachRenderBuffer(*m_rbo);
}

uint32_t FrameRender::GetAttachTextureId() const{
    return m_render_texture->GetId();
}

} // namespace Platform::GL
