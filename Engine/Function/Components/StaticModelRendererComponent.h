/**
 * @file StaticModelRendererComponent.h
 * @author Echo
 * @Date 24-3-7
 * @brief StaticModelRenderer的Component封装
 */

#ifndef STATICMODELRENDER_H
#define STATICMODELRENDER_H
#include <memory>
#include <string>
#include <vector>

#include "OpenGL/Shader.h"
#include "OpenGL/ShaderProgram.h"
#include "RendererComponentBase.h"

namespace Resource {
class Model;
}

class StaticMeshRenderer;

class StaticModelRendererComponent : public RendererComponentBase {
  using ShaderProgram = Platform::GL::ShaderProgram;

public:
  explicit StaticModelRendererComponent(std::wstring_view model_path = L"", std::wstring_view vertex_shader_path = L"", std::wstring_view fragment_shader_path = L"");

  ~StaticModelRendererComponent() override = default;

  void Initialize() override;
  void TickRender() override;
  void Deinitialize() override;

  /** 设置模型路径 */
  void SetModelPath(const std::wstring &path);

  std::wstring GetModelPath() const { return m_model_path; }

private:
  std::wstring m_model_path;           // 模型路径
  std::wstring m_vertex_shader_path;   // 顶点着色器路径
  std::wstring m_fragment_shader_path; // 片段着色器路径

  std::vector<StaticMeshRenderer*> m_renders; // 模型所有的MeshRenderer
  std::shared_ptr<Resource::Model> m_model;  // 模型本身
  // TODO: 封装Material
  ShaderProgram m_shader_program; // ShaderProgram
  bool m_initialized{false};
};

#endif // STATICMODELRENDER_H
