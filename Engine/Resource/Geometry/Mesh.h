/**
 * @file Mesh.h
 * @author Echo
 * @Date 2024/3/6
 * @brief 网格定义
 */

#ifndef ZEPHYR_MESH_H
#define ZEPHYR_MESH_H

#include <string>
#include <vector>

#include "Math/BasicType.h"
#include "OpenGL/Texture.h"

namespace Resource {

struct Vertex {
  Vector3 position;
  Vector3 normal;
  Vector2 tex_coords;
};

// TODO: 集中资源管理类
class Mesh {
  friend class Model;

public:
  inline const std::vector<Vertex> &GetVertices() const { return m_vertices; }
  inline const std::vector<uint32_t> &GetIndices() const { return m_indices; }
  inline const std::vector<Platform::GL::Texture> &GetTextures() const { return m_textures; }

private:
  std::vector<Vertex> m_vertices;
  std::vector<uint32_t> m_indices;
  std::vector<Platform::GL::Texture> m_textures;
};

}

#endif // ZEPHYR_MESH_H
