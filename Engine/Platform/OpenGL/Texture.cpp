/**
 * @file Texture.cpp
 * @author Echo
 * @Date 2024/3/2
 * @brief
 */

#include "Texture.h"

#include "Geometry/Image.h"
#include "Logger/Logger.h"

using namespace Resource;

namespace Platform::GL {

Texture::Texture() { glGenTextures(1, &m_id); }

Texture::~Texture() { glDeleteTextures(1, &m_id); }

uint32_t Texture::GetId() const { return m_id; }

void Texture::Bind(const uint32_t target) {
  glBindTexture(target, m_id);
  m_target = target;
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
  glTexParameteri(target, pname, param);
  return *this;
}

const TextureParam &Texture::GetParam() const { return m_param; }

uint32_t Texture::GetTarget() const { return m_target; }

void Texture::Apply() const {
  if (m_param.data == nullptr) {
    ZEPHYR_LOG_ERROR("Texture data is nullptr");
    return;
  }
  glTexImage2D(m_target, m_param.level, m_param.internal_format, m_param.width,
               m_param.height, m_param.border, m_param.format, m_param.type,
               m_param.data);
  // glGenerateMipmap(m_target);
}

void Texture::ApplyFrameBuffer() {
  Bind();
  glTexImage2D(m_target, m_param.level, m_param.internal_format, m_param.width,
               m_param.height, m_param.border, m_param.format, m_param.type,
               nullptr);
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

Texture &Texture::SetUsage(const ETextureUsage usage) {
  m_usage = usage;
  return *this;
}

} // namespace Platform::GL
