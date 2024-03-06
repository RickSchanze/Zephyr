/**
 * @file Image.cpp
 * @author Echo
 * @Date 2024/3/6
 * @brief
 */

#include "Image.h"

#include <filesystem>

#include "stb_image.h"

#include "Logger/Logger.h"

namespace fs = std::filesystem;

namespace Resource {
void Image::Load(const std::string &path) {
  if (!fs::exists(path)) {
    ZEPHYR_LOG_ERROR("Image not found: {}", path);
    return;
  }

  m_path = path;
  uint8_t *data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
  if (!data) {
    ZEPHYR_LOG_ERROR("Failed to load image: {}", path);
    return;
  }
  m_data = data;
}

bool Image::IsValid() const {
  return m_data != nullptr;
}

Image *Image::Create(const std::string &path) {
  if (!fs::exists(path)) {
    ZEPHYR_LOG_ERROR("Image not found: {}", path);
    return nullptr;
  }
  auto rtn = new Image();
  rtn->Load(path);
  return rtn;
}

Image::~Image() {
  if (m_data) {
    stbi_image_free(m_data);
  }
}
} // namespace Resource
