/**
 * @file StaticMeshRenderer.cpp
 * @author Echo
 * @Date 2024/3/4
 * @brief
 */

#include "StaticMeshRenderer.h"

#include "Geometry/Mesh.h"
#include "Geometry/Model.h"
#include "OpenGL/ShaderProgram.h"
#include "OpenGL/VertexArrayObject.h"
#include "imgui.h"

#include <GL/gl.h>

using namespace Platform::GL;

StaticMeshRenderer::StaticMeshRenderer(Resource::Mesh *mesh) {
  m_mesh = mesh;
}

void StaticMeshRenderer::Draw(const Platform::GL::ShaderProgram &shader) {
  shader.Use();
  m_vao->Bind();
  // TODO: 优化这里的程序结构
  // render the loaded model
  glActiveTexture(GL_TEXTURE0);
  // glUniform1i(glGetUniformLocation(shader.GetID(), "texture_diffuse1"), 0);
  m_textures[0].Bind();
  auto a = glGetError();

  glDrawElements(GL_TRIANGLES, static_cast<int32_t>(m_mesh->GetIndices().size()), GL_UNSIGNED_INT, 0);
}

void StaticMeshRenderer::InitializeObjects() {
  if (m_mesh) {
    m_vao->Bind();
    SetVertexBufferData(&m_mesh->GetVertices()[0], m_mesh->GetVertices().size() * sizeof(Resource::Vertex));
    SetElementBufferData(&m_mesh->GetIndices()[0], m_mesh->GetIndices().size() * sizeof(uint32_t));
    this->BindVertexAttributePointer(3, offsetof(Resource::Vertex, position))   // position
        .BindVertexAttributePointer(3, offsetof(Resource::Vertex, normal))      // normal
        .BindVertexAttributePointer(2, offsetof(Resource::Vertex, tex_coords)); // tex_coords
    m_vao->Unbind();

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
