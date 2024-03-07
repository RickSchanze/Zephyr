/**
 * @file ShaderProgram.h
 * @author Echo
 * @Date 2024/3/6
 * @brief
 */

#ifndef ZEPHYR_SHADERPROGRAM_H
#define ZEPHYR_SHADERPROGRAM_H

#include <cstdint>

#ifndef __glad_h_
#include "glad/glad.h"
#endif

#include "Math/BasicType.h"

#include <string>

namespace Platform::GL {

class ShaderProgram {
public:
  ShaderProgram();
  ~ShaderProgram();

  /** 链接Shader */
  void Link() const;

  /** 为此Program附加Shader */
  template <typename... Shaders>
  void AttachShader(const Shaders &...shaders) {
    (glAttachShader(m_id, shaders.GetId()), ...);
  }

  /** 使用当前ShaderProgram */
  void Use() const;

  void SetMatrix4(const std::string &name, const Matrix4 &matrix) const;

  uint32_t GetID() const { return m_id; }

private:
  uint32_t m_id;
};
} // namespace Platform::GL

#endif // ZEPHYR_SHADERPROGRAM_H
