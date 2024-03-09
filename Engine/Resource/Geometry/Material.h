/**
 * @file Material.h
 * @author Echo
 * @Date 24-3-9
 * @brief
 */

#ifndef MATERIAL_H
#define MATERIAL_H
#include "AssetBase.h"

namespace Platform::GL {
class ShaderProgram;
}

namespace Resource {
class Material : public AssetBase {
public:
  Material() = default;
  ~Material() override = default;

  void Use(const Platform::GL::ShaderProgram& shader);

  void SetDiffuseTexturePath(const std::wstring &path) { m_diffuse_texture_path1 = path; }
  void SetNormalTexturePath(const std::wstring &path) { m_normal_texture_path1 = path; }
  void SetSpecularTexturePath(const std::wstring &path) { m_specular_texture_path1 = path; }

protected:
  void ActiveTexture(const std::string &texture_name, const std::wstring &texture_path, const Platform::GL::ShaderProgram& shader);

private:
  std::wstring m_shader_path;
  std::wstring m_diffuse_texture_path1;
  std::wstring m_normal_texture_path1;
  std::wstring m_specular_texture_path1;
  int32_t m_texture_enable_count = 0;
};
} // namespace Resource

#endif // MATERIAL_H
