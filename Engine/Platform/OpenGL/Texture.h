/**
 * @file Texture.h
 * @author Echo
 * @Date 2024/3/2
 * @brief
 */

#ifndef ZEPHYR_TEXTURE_H
#define ZEPHYR_TEXTURE_H

#include "Geometry/Mesh.h"

#include <cstdint>
#include <unordered_map>

#ifndef __glad_h_
#include "glad/glad.h"
#endif

namespace Resource {
class Image;
}

namespace Platform::GL {

struct TextureParam {
  int32_t level = 0;                // 纹理的mipmap级别,如果target为GL_TEXTURE_RECTANGLE或GL_PROXY_TEXTURE_RECTANGLE，则level必须为0。
  int32_t internal_format = GL_RGB; // 纹理的内部格式
  int32_t width = 0;                // 纹理的宽度
  int32_t height = 0;               // 纹理的高度
  int32_t border = 0;               // 纹理的边框宽度(必须为0)
  int32_t format = GL_RGB;          // 像素数据的格式
  int32_t type = GL_UNSIGNED_BYTE;  // 像素数据的数据类型
  uint8_t *data = nullptr;          // 像素数据
};

class Texture {
  using ETextureUsage = Resource::ETextureUsage;

public:
  Texture();
  ~Texture();

  /** 获取id */
  uint32_t GetId() const;
  /**
   * 绑定texture
   * @param target 纹理类型 默认GL_TEXTURE_2D
   */
  void Bind(uint32_t target = GL_TEXTURE_2D)const;

  /**
   * 设定texture参数
   * @note 内部调用glTexImage2D
   */
  Texture &SetImageParam(const TextureParam &param);

  /**
   * 设置纹理参数
   * @param image 一个已经加载好的Image
   * @return
   */
  Texture &SetImageParam(const Resource::Image &image);

  /**
   * 设定纹理参数
   * @note 内部调用glTexParameteri
   * @param pname 参数名
   * @param param 参数值
   * @param target 参数应用的纹理类型
   * @return 返回自身
   */
  Texture &SetParam(int32_t pname, int32_t param,
                    uint32_t target = GL_TEXTURE_2D);

  /**
   * 获取纹理参数
   * @return
   */
  const TextureParam &GetParam() const;

  /** 调用glTexImage2D生成纹理 */
  void Apply();

  /** 为FrameBuffer生成纹理 */
  void ApplyFrameBuffer();

  bool HasApplied() const;

  static std::shared_ptr<Texture> GetTexture(const std::wstring &image_path);
  static std::shared_ptr<Texture> GetOrCreateTexture(const std::wstring &image_path);

private:
  uint32_t m_id = 0;
  TextureParam m_param;
  bool m_has_applied = false;
  inline static std::unordered_map<std::wstring, std::shared_ptr<Texture>> s_image_texture_map; // 图像路径和Texture的对应
};

} // namespace Platform::GL


#endif // ZEPHYR_TEXTURE_H
