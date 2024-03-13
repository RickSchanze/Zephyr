/**
 * @file StaticMeshRenderer.h
 * @author Echo
 * @Date 2024/3/4
 * @brief
 */

#ifndef ZEPHYR_STATICMESHRENDERER_H
#define ZEPHYR_STATICMESHRENDERER_H

#include "BaseRenderer.h"
#include "Geometry/Material.h"
#include "Geometry/Mesh.h"
#include "OpenGL/Shader.h"

namespace Platform::GL {
class ShaderProgram;
}

namespace Resource {
class Mesh;
}

class StaticMeshRenderer : public BaseRenderer {
  typedef Platform::GL::ShaderProgram ShaderProgram;
  typedef Platform::GL::VertexShader VertexShader;
  typedef Platform::GL::FragmentShader FragmentShader;
  typedef Resource::Material Material;

public:
  /**
   * 构造函数
   * @param mesh 一个Mesh
   */
  explicit StaticMeshRenderer(Resource::Mesh* mesh);

  void Draw(const Platform::GL::ShaderProgram &shader) override;

  /** 初始化各种OpenGL Object */
  void InitializeObjects();

protected:
  Resource::Mesh *m_mesh;
  std::shared_ptr<Material> m_material;
  bool m_initialized{false};
};

#endif // ZEPHYR_STATICMESHRENDERER_H
