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
#include "Utils/TypeConverter.h"

namespace fs = std::filesystem;

namespace Resource {
void Image::Load() {
  if (!fs::exists(m_path)) {
    ZEPHYR_LOG_ERROR("Image not found: {}", m_path);
    return;
  }
  const std::string path = TypeConverter::UTF16WString2UTF8(m_path);
  uint8_t *data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
  if (!data) {
    ZEPHYR_LOG_ERROR("Failed to load image: {}", m_path);
    return;
  }
  m_data = data;
}

bool Image::IsValid() const {
  return m_data != nullptr;
}

std::shared_ptr<Image> Image::Create(const std::wstring &path) {
  if (!fs::exists(path)) {
    ZEPHYR_LOG_ERROR("Image not found: {}", path);
    return nullptr;
  }
  auto rtn = std::make_shared<Image>(path);
  rtn->Load();
  return rtn;
}

Image::Image(const std::wstring &path) {
  m_path = path;
}

Image::~Image() {
  if (m_data) {
    stbi_image_free(m_data);
  }
}
} // namespace Resource
