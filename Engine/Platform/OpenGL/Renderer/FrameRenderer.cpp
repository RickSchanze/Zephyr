/**
 * @file FrameRenderer.cpp
 * @author Echo
 * @Date 24-3-3
 * @brief
 */

#include "FrameRenderer.h"

#include <iostream>

namespace Platform::GL
{

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

bool FrameRenderer::Initialize(const int width, const int height)
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
    renderer = std::make_unique<StaticMeshRenderer>(R"(C:\Users\Echo\BaiduSyncDisk\Work\Projects\Zephyr\Engine\Platform\OpenGL\Shader\test.vert)", R"(C:\Users\Echo\BaiduSyncDisk\Work\Projects\Zephyr\Engine\Platform\OpenGL\Shader\test.frag)");
    renderer->SetVertexBufferData(vertices, sizeof(vertices), GL_STATIC_DRAW, GL_ARRAY_BUFFER);
    renderer->BindVertexAttributePointer(3, GL_FLOAT, false);
    renderer->SetElementBufferData(indices, sizeof(indices));

    return m_fbo->Check();
}

void FrameRenderer::Draw() const
{
    m_fbo->Bind();
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    renderer->Draw();
    m_fbo->Unbind();
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
}

void FrameRenderer::Resize(const int width, const int height) const
{
    m_render_texture->Bind();
    m_render_texture->SetImageParam({.width = width, .height = height});
    m_fbo->AttachTexture(*m_render_texture);

    m_rbo->Bind();
    m_rbo->AllocateStorage(width, height);
    m_fbo->AttachRenderBuffer(*m_rbo);
}

uint32_t FrameRenderer::GetAttachTextureId() const{
    return m_render_texture->GetId();
}

} // namespace Platform::GL
