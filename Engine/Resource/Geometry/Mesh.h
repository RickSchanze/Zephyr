/**
 * @file Mesh.h
 * @author Echo
 * @Date 2024/3/6
 * @brief 网格定义
 */

#ifndef ZEPHYR_MESH_H
#define ZEPHYR_MESH_H

#include "Image.h"

#include <vector>

#include "Math/BasicType.h"

#include <memory>

namespace Resource {

struct Vertex {
  Vector3 position;
  Vector3 normal;
  Vector2 tex_coords;
};

enum class ETextureUsage : uint8_t {
  Diffuse,  // 漫反射贴图
  Specular, // 镜面反射贴图
  Normal,   // 法线贴图
  Height,   // 高度贴图

  Max,      // 未初始化
};

/** 用于保存具体数据的texture */
struct Texture {
  std::shared_ptr<Image> image;
  ETextureUsage usage = ETextureUsage::Max;
};

// TODO: 集中资源管理类
class Mesh {
  friend class Model;

public:
  inline const std::vector<Vertex> &GetVertices() const { return m_vertices; }
  inline const std::vector<uint32_t> &GetIndices() const { return m_indices; }
  inline const std::vector<Texture> &GetTextures() const { return m_textures; }

private:
  std::vector<Vertex> m_vertices;
  std::vector<uint32_t> m_indices;
  std::vector<Texture> m_textures;
};

}

#endif // ZEPHYR_MESH_H
