/**
 * @file Material.cpp
 * @author Echo
 * @Date 24-3-9
 * @brief
 */

#include "Material.h"

#include "OpenGL/ShaderProgram.h"
#include "OpenGL/Texture.h"
#include "Material.generated.h"

void Resource::Material::Use(const Platform::GL::ShaderProgram &shader) {
  // shader.Use();
  ActiveTexture("diffuse_texture", m_diffuse_texture_path1, shader);
  ActiveTexture("normal_texture", m_normal_texture_path1, shader);
  ActiveTexture("specular_texture", m_specular_texture_path1, shader);
  m_texture_enable_count = 0;
  glActiveTexture(GL_TEXTURE0);
}

void Resource::Material::ActiveTexture(const std::string &texture_name, const std::wstring &texture_path, const Platform::GL::ShaderProgram &shader) {
  if (!texture_path.empty()) {
    const auto diffuse_texture = Platform::GL::Texture::GetOrCreateTexture(texture_path);
    if (!diffuse_texture->HasApplied()) {
      diffuse_texture->SetParam(GL_TEXTURE_WRAP_S, GL_REPEAT)
          .SetParam(GL_TEXTURE_WRAP_T, GL_REPEAT)
          .SetParam(GL_TEXTURE_MIN_FILTER, GL_LINEAR)
          .SetParam(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      diffuse_texture->Apply();
    }
    glActiveTexture(GL_TEXTURE0 + m_texture_enable_count);
    shader.SetInt(texture_name + std::to_string(m_texture_enable_count), m_texture_enable_count);
    diffuse_texture->Bind();
    m_texture_enable_count++;
  }
}
