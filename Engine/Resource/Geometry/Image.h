/**
 * @file Image.h
 * @author Echo
 * @Date 2024/3/6
 * @brief
 */

#ifndef ZEPHYR_IMAGE_H
#define ZEPHYR_IMAGE_H

#include <cstdint>
#include <memory>
#include <string>

namespace Resource {

// TODO: 加载优化，集中管理，加载相同的直接返回
class Image {
public:
  ~Image();

  inline std::string GetPath() const { return m_path; }
  inline int GetWidth() const { return m_width; }
  inline int GetHeight() const { return m_height; }
  inline int GetChannels() const { return m_channels; }
  inline uint8_t *GetData() const { return m_data; }

  /** 此图像是否有效 */
  bool IsValid() const;

  /**
   * 创建并一个图像
   * @param path 图像路径
   * @return 如果加载失败或路径不存在则返回nullptr
   */
  static std::shared_ptr<Image> Create(const std::string &path);

  /**
   * 加载一个图像
   * @param path 图像路径
   */
  void Load(const std::string &path);

private:
  std::string m_path;        // 图像路径
  int m_width = 0;           // 图像宽度
  int m_height = 0;          // 图像高度
  int m_channels = 0;        // 图像通道数
  uint8_t *m_data = nullptr; // 图像数据
};
} // namespace Resource

#endif // ZEPHYR_IMAGE_H
