/**
 * @file StaticMeshRenderer.h
 * @author Echo
 * @Date 2024/3/4
 * @brief
 */

#ifndef ZEPHYR_STATICMESHRENDERER_H
#define ZEPHYR_STATICMESHRENDERER_H

#include "BaseRenderer.h"
#include "Geometry/Mesh.h"
#include "OpenGL/Shader.h"

namespace Platform::GL {
class ShaderProgram;
}

namespace Resource {
class Model;
}

class StaticModelRenderer : public BaseRenderer {
  typedef Platform::GL::ShaderProgram ShaderProgram;
  typedef Platform::GL::VertexShader VertexShader;
  typedef Platform::GL::FragmentShader FragmentShader;

public:
  /**
   * 构造函数
   * @param model_path 模型文件路径
   * @param vertex_shader_path 顶点着色器路径
   * @param fragment_shader_path 片元着色器路径
   */
  StaticModelRenderer(const char* model_path, const char *vertex_shader_path, const char *fragment_shader_path);

  void Draw() override;

  /** 初始化各种OpenGL Object */
  bool InitializeObjects();

  ~StaticModelRenderer() override;

protected:
  VertexShader *m_vertex_shader;
  FragmentShader *m_fragment_shader;
  ShaderProgram *m_shader_program{};
  std::shared_ptr<Resource::Model> m_model;
  bool m_initialized{false};
};

#endif // ZEPHYR_STATICMESHRENDERER_H
