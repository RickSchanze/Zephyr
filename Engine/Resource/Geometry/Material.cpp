/**
 * @file Material.cpp
 * @author Echo 
 * @Date 24-3-9
 * @brief 
 */

#include "Material.h"

#include "OpenGL/ShaderProgram.h"
#include "OpenGL/Texture.h"

void Resource::Material::Use(const Platform::GL::ShaderProgram &shader) {
  shader.Use();
  ActiveTexture("diffuse_texture", m_diffuse_texture_path1, shader);
  ActiveTexture("normal_texture", m_normal_texture_path1, shader);
  ActiveTexture("specular_texture", m_specular_texture_path1, shader);
}

void Resource::Material::ActiveTexture(const std::string &texture_name, const std::wstring &texture_path, const Platform::GL::ShaderProgram &shader) {
  if (!texture_path.empty()) {
    const auto diffuse_texture = Platform::GL::Texture::GetOrCreateTexture(texture_path);
    if (!diffuse_texture->HasApplied()) {
      diffuse_texture->Apply();
    }
    glActiveTexture(m_texture_enable_count);
    shader.SetInt(texture_name + std::to_string(0), m_texture_enable_count);
    m_texture_enable_count++;
  }
}

