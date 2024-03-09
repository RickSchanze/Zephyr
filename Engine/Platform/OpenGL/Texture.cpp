/**
 * @file Texture.cpp
 * @author Echo
 * @Date 2024/3/2
 * @brief
 */

#include "Texture.h"

#include "AssetManager.h"
#include "Geometry/Image.h"
#include "Logger/Logger.h"

using namespace Resource;

namespace Platform::GL {

Texture::Texture() { glGenTextures(1, &m_id); }

Texture::~Texture() {
  glDeleteTextures(1, &m_id);
}

uint32_t Texture::GetId() const { return m_id; }

void Texture::Bind(const uint32_t target) const {
  glBindTexture(GL_TEXTURE_2D, m_id);
}

Texture &Texture::SetImageParam(const TextureParam &param) {
  if (param.width <= 0 || param.height <= 0) {
    ZEPHYR_LOG_ERROR("Width or height is invalid");
    return *this;
  }
  m_param = param;
  return *this;
}

Texture &Texture::SetParam(const int32_t pname, const int32_t param,
                           const uint32_t target) {
  // Bind();
  glTexParameteri(GL_TEXTURE_2D, pname, param);
  return *this;
}

const TextureParam &Texture::GetParam() const { return m_param; }

void Texture::Apply() {
  if (m_param.data == nullptr) {
    ZEPHYR_LOG_ERROR("Texture data is nullptr");
    return;
  }
  Bind();
  glTexImage2D(GL_TEXTURE_2D, m_param.level, m_param.internal_format, m_param.width,
               m_param.height, m_param.border, m_param.format, m_param.type,
               m_param.data);
  // glGenerateMipmap(GL_TEXTURE_2D);
  m_has_applied = true;
}

void Texture::ApplyFrameBuffer() {
  Bind();
  glTexImage2D(GL_TEXTURE_2D, m_param.level, m_param.internal_format, m_param.width,
               m_param.height, m_param.border, m_param.format, m_param.type,
               nullptr);
  m_has_applied = true;
}

bool Texture::HasApplied() const {
  return m_has_applied;
}

std::shared_ptr<Texture> Texture::GetTexture(const std::wstring &image_path) {
  if (s_image_texture_map.contains(image_path)) {
    return s_image_texture_map[image_path];
  }
  return nullptr;
}

std::shared_ptr<Texture> Texture::GetOrCreateTexture(const std::wstring &image_path) {
  auto tex = GetTexture(image_path);
  if (tex)
    return tex;
  const auto image = AssetManager::Request<Image>(image_path);
  auto new_texture = std::make_shared<Texture>();
  new_texture->SetImageParam(*image);
  return new_texture;
}

Texture &Texture::SetImageParam(const Image &image) {
  if (!image.IsValid()) {
    ZEPHYR_LOG_ERROR("Set texture from invalid image.");
    return *this;
  }
  m_param.width = image.GetWidth();
  m_param.height = image.GetHeight();
  m_param.data = &image.GetData()[0];
  m_param.format = image.GetChannels() == 3 ? GL_RGB : GL_RGBA;
  m_param.internal_format = m_param.format;
  SetImageParam(m_param);
  return *this;
}

} // namespace Platform::GL
