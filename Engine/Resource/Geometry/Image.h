/**
 * @file Image.h
 * @author Echo
 * @Date 2024/3/6
 * @brief
 */

#ifndef ZEPHYR_IMAGE_H
#define ZEPHYR_IMAGE_H

#include "AssetBase.h"

#include <cstdint>
#include <memory>
#include <string>

namespace Resource {

// TODO: 加载优化，集中管理，加载相同的直接返回
class Image : public AssetBase {
public:
  Image() = default;
  explicit Image(const std::wstring& path);

  ~Image() override;

  std::wstring GetPath() const { return m_path; }
  int GetWidth() const { return m_width; }
  int GetHeight() const { return m_height; }
  int GetChannels() const { return m_channels; }
  uint8_t *GetData() const { return m_data; }


  /** 此图像是否有效 */
  bool IsValid() const;

  /**
   * 创建并一个图像
   * @param path 图像路径
   * @return 如果加载失败或路径不存在则返回nullptr
   */
  static std::shared_ptr<Image> Create(const std::wstring &path);

  /**
   * 加载一个图像
   */
  void Load() override;

  bool IsValid() override;

private:
  int m_width = 0;           // 图像宽度
  int m_height = 0;          // 图像高度
  int m_channels = 0;        // 图像通道数
  uint8_t *m_data = nullptr; // 图像数据
};
} // namespace Resource

#endif // ZEPHYR_IMAGE_H
