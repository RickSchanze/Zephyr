/**
 * @file StaticMeshRenderer.cpp
 * @author Echo
 * @Date 2024/3/4
 * @brief
 */

#include "StaticModelRenderer.h"

#include "Geometry/Model.h"
#include "OpenGL/ShaderProgram.h"
#include "OpenGL/VertexArrayObject.h"

using namespace Platform::GL;

StaticModelRenderer::~StaticModelRenderer() {
  delete m_vertex_shader;
  delete m_fragment_shader;
  delete m_shader_program;
}

StaticModelRenderer::StaticModelRenderer(const char *model_path, const char *vertex_shader_path, const char *fragment_shader_path) {
  m_vertex_shader = new VertexShader(vertex_shader_path);
  m_fragment_shader = new FragmentShader(fragment_shader_path);
  m_model = Resource::Model::Create(model_path);
}

void StaticModelRenderer::Draw() {
  if (!m_initialized) {
    m_vertex_shader->Initialize();
    m_fragment_shader->Initialize();
    m_shader_program = new ShaderProgram();
    m_shader_program->AttachShader(*m_vertex_shader, *m_fragment_shader);
    m_shader_program->Link();
    m_initialized = true;
  }

  m_shader_program->Use();
  m_vao->Bind();
  glDrawElements(GL_TRIANGLES, m_vertex_count, GL_UNSIGNED_INT, nullptr);

  auto a = glGetError();
}

bool StaticModelRenderer::InitializeObjects() {
  if (!m_model) {
    ZEPHYR_LOG_ERROR("Model not complete.");
    return false;
  }
  if (!m_model->IsValid()) {
    ZEPHYR_LOG_ERROR("Model not valid.");
    return false;
  }
  // 初始化VAO,VBO,EBO
  // TODO: 初始化VAO VBO EBO
}
