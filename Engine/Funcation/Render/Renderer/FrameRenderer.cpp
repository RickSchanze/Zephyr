/**
 * @file FrameRenderer.cpp
 * @author Echo
 * @Date 24-3-3
 * @brief
 */

#include "FrameRenderer.h"

#include "Components/StaticModelRendererComponent.h"

using namespace Platform::GL;

bool FrameRenderer::Initialize(const int width, const int height) {
  // 创建各个需要的对象
  m_render_texture = std::make_unique<Texture>();
  m_rbo = std::make_unique<RenderBufferObject>();
  m_fbo = std::make_unique<FrameBufferObject>();

  // 配置各个对象
  // 配置texture
  m_render_texture->SetImageParam({.width = width, .height = height}).ApplyFrameBuffer();
  // 配置rbo
  m_rbo->Bind();
  m_rbo->AllocateStorage(width, height);
  // 配置fbo
  m_fbo->Bind();
  m_fbo->AttachTexture(*m_render_texture);
  m_fbo->AttachRenderBuffer(*m_rbo);

  m_static_model_renderer = std::make_unique<StaticModelRendererComponent>(LR"(C:\Users\Echo\Documents\BaiduSyncdisk\aaa\nanosuit.obj)",
                                                                           LR"(C:\Users\Echo\BaiduSyncDisk\Work\Projects\Zephyr\Engine\Resource\Shaders\test.vert)",
                                                                           LR"(C:\Users\Echo\BaiduSyncDisk\Work\Projects\Zephyr\Engine\Resource\Shaders\test.frag)");
  m_static_model_renderer->Initialize();

  return m_fbo->Check();
}

void FrameRenderer::Draw() const {
  m_fbo->Bind();
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(.1f, .1f, .1f, 1.0f);

  m_static_model_renderer->TickRender();

  m_fbo->Unbind();
  glDisable(GL_DEPTH_TEST);
}

void FrameRenderer::Resize(const int width, const int height) const {
  m_render_texture->Bind();
  m_render_texture->SetImageParam({.width = width, .height = height});
  m_fbo->AttachTexture(*m_render_texture);

  m_rbo->Bind();
  m_rbo->AllocateStorage(width, height);
  m_fbo->AttachRenderBuffer(*m_rbo);
}

uint32_t FrameRenderer::GetAttachTextureId() const {
  return m_render_texture->GetId();
}
