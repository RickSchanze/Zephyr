/**
 * @file StaticMeshRenderer.cpp
 * @author Echo
 * @Date 2024/3/4
 * @brief
 */

#include "StaticMeshRenderer.h"

#include "AssetManager.h"
#include "Geometry/Mesh.h"
#include "Geometry/Model.h"
#include "OpenGL/ElementBufferObject.h"
#include "OpenGL/ShaderProgram.h"
#include "OpenGL/VertexArrayObject.h"
#include "imgui.h"

#include <GL/gl.h>

using namespace Platform::GL;

StaticMeshRenderer::StaticMeshRenderer(Resource::Mesh *mesh) {
  m_mesh = mesh;
}

void StaticMeshRenderer::Draw(const Platform::GL::ShaderProgram &shader) {
  m_material->Use(shader);

  m_vao->Bind();
  glDrawElements(GL_TRIANGLES, static_cast<int32_t>(m_mesh->GetIndices().size()), GL_UNSIGNED_INT, nullptr);
  m_vao->Unbind();

  // 恢复原状
  glActiveTexture(GL_TEXTURE0);
}

void StaticMeshRenderer::InitializeObjects() {
  static int a = 0;
  if (m_mesh) {
    SetVertexBufferData(&m_mesh->GetVertices()[0], m_mesh->GetVertices().size() * sizeof(Resource::Vertex));
    SetElementBufferData(&m_mesh->GetIndices()[0], static_cast<int>(m_mesh->GetIndices().size() * sizeof(unsigned int)));
    this->BindVertexAttributePointer(3, offsetof(Resource::Vertex, position))   // position
        .BindVertexAttributePointer(3, offsetof(Resource::Vertex, normal))      // normal
        .BindVertexAttributePointer(2, offsetof(Resource::Vertex, tex_coords)); // tex_coords
    // 绑定texture
    // TODO: 优化这里的程序结构
    auto textures = m_mesh->GetTextures();
    m_material = AssetManager::Get().Request<Material>(std::to_wstring(a++));
    for (const auto &[image, usage] : textures) {
      switch (usage) {
      case Resource::ETextureUsage::Diffuse:
        m_material->SetDiffuseTexturePath(image->GetPath());
        break;
      case Resource::ETextureUsage::Specular:
        m_material->SetSpecularTexturePath(image->GetPath());
        break;
      case Resource::ETextureUsage::Normal:
        m_material->SetNormalTexturePath(image->GetPath());
        break;
      default:
        break;
      }
    }

    m_initialized = true;
  }
}
