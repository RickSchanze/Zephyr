/**
 * @file StaticModelRendererComponent.cpp
 * @author Echo
 * @Date 24-3-7
 * @brief
 */

#include "StaticModelRendererComponent.h"

#include "imgui.h"
#include "Geometry/Model.h"
#include "Render/Renderer/StaticMeshRenderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace Platform::GL;

StaticModelRendererComponent::StaticModelRendererComponent(std::wstring_view model_path, std::wstring_view vertex_shader_path, std::wstring_view fragment_shader_path) {
  m_model_path = model_path;
  m_vertex_shader_path = vertex_shader_path;
  m_fragment_shader_path = fragment_shader_path;
}

void StaticModelRendererComponent::Initialize() {
  // TODO: 统一管理
  m_model = Resource::Model::Create(m_model_path);
  if (m_model) {
    auto &meshs = m_model->GetMeshes();
    for (auto &mesh : meshs) {
      auto *render = new StaticMeshRenderer(mesh);
      render->InitializeObjects();
      m_renders.push_back(render);
    }

    VertexShader vertex_shader(m_vertex_shader_path);
    vertex_shader.Initialize();
    FragmentShader fragment_shader(m_fragment_shader_path);
    fragment_shader.Initialize();
    if (vertex_shader.IsValid()) {
      m_shader_program.AttachShader(vertex_shader);
    }
    if (fragment_shader.IsValid()) {
      m_shader_program.AttachShader(fragment_shader);
    }
    // 初始化Shader
    m_shader_program.Link();
    m_initialized = true;
  }
}

glm::vec3 translate(0.0f, 0.0f, 0.0f);
glm::vec3 scale(1.0f, 1.0f, 1.0f);

void StaticModelRendererComponent::TickRender() {
  static auto proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
  static auto view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  auto model = glm::mat4(1.0f);
  model = glm::translate(model, translate); // translate it down so it's at the center of the scene
  model = glm::scale(model, scale);     // it's a bit too big for our scene, so scale it down

  m_shader_program.Use();
  ImGui::SliderFloat3("translate", &translate.x, -100.0f, 100.0f);
  ImGui::SliderFloat3("scale", &scale.x, 0.0f, 10.0f);
  m_shader_program.SetMatrix4("projection", proj);
  m_shader_program.SetMatrix4("view", view);
  m_shader_program.SetMatrix4("model", model);
  auto err1 = glGetError();
  for (auto *render : m_renders) {
    render->Draw(m_shader_program);
  }
}

void StaticModelRendererComponent::Deinitialize() {
}

void StaticModelRendererComponent::SetModelPath(const std::wstring &path) {
  m_model_path = path;
}
