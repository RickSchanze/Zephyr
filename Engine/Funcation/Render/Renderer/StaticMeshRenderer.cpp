/**
 * @file StaticMeshRenderer.cpp
 * @author Echo
 * @Date 2024/3/4
 * @brief
 */

#include "StaticMeshRenderer.h"

#include "Geometry/Mesh.h"
#include "Geometry/Model.h"
#include "OpenGL/ElementBufferObject.h"
#include "OpenGL/ShaderProgram.h"
#include "OpenGL/VertexArrayObject.h"
#include "OpenGL/VertexBufferObject.h"
#include "imgui.h"

#include <GL/gl.h>

using namespace Platform::GL;

StaticMeshRenderer::StaticMeshRenderer(Resource::Mesh *mesh) {
  m_mesh = mesh;
}

void StaticMeshRenderer::Draw(const Platform::GL::ShaderProgram &shader) {
  uint32_t diffuse_number = 0;
  uint32_t specular_number = 0;
  uint32_t normal_number = 0;
  uint32_t height_number = 0;

  for (int32_t i = 0; i < m_textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    std::string name;
    switch (m_textures[i].GetUsage()) {
    case Resource::ETextureUsage::Diffuse:
      name = std::string("texture_diffuse") + std::to_string(diffuse_number++);
      break;
    case Resource::ETextureUsage::Normal:
      name = std::string("texture_normal") + std::to_string(normal_number++);
      break;
    case Resource::ETextureUsage::Specular:
      name = std::string("texture_specular") + std::to_string(specular_number++);
      break;
    case Resource::ETextureUsage::Height:
      name = std::string("texture_height") + std::to_string(height_number++);
      break;
    default:;
    }
    // 设置纹理采样器
    shader.SetInt(name, i);
    m_textures[i].Bind();
  }

  m_vao->Bind();
  glDrawElements(GL_TRIANGLES, static_cast<int32_t>(m_mesh->GetIndices().size()), GL_UNSIGNED_INT, nullptr);
  m_vao->Unbind();

  // 恢复原状
  glActiveTexture(GL_TEXTURE0);
}

void StaticMeshRenderer::InitializeObjects() {
  if (m_mesh) {
    SetVertexBufferData(&m_mesh->GetVertices()[0], m_mesh->GetVertices().size() * sizeof(Resource::Vertex));
    SetElementBufferData(&m_mesh->GetIndices()[0], m_mesh->GetIndices().size() * sizeof(unsigned int));
    auto a = glGetError();
    this->BindVertexAttributePointer(3, offsetof(Resource::Vertex, position))   // position
        .BindVertexAttributePointer(3, offsetof(Resource::Vertex, normal))      // normal
        .BindVertexAttributePointer(2, offsetof(Resource::Vertex, tex_coords)); // tex_coords
    // 绑定texture
    // TODO: 优化这里的程序结构
    auto textures = m_mesh->GetTextures();
    for (auto &[image, usage] : textures) {
      if (usage == Resource::ETextureUsage::Diffuse) {
        auto t = Platform::GL::Texture();
        t.SetImageParam(*image)
            .SetUsage(usage)
            .SetParam(GL_TEXTURE_MIN_FILTER, GL_LINEAR)
            .SetParam(GL_TEXTURE_MAG_FILTER, GL_LINEAR)
            .SetParam(GL_TEXTURE_WRAP_S, GL_REPEAT)
            .SetParam(GL_TEXTURE_WRAP_T, GL_REPEAT)
            .Apply();
        m_textures.push_back(t);
      }
    }

    m_initialized = true;
  }
}
