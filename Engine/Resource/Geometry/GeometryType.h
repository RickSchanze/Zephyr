/**
 * @file GeometryType.h
 * @author Echo
 * @Date 2024/3/6
 * @brief
 */

#ifndef ZEPHYR_GEOMETRYTYPE_H
#define ZEPHYR_GEOMETRYTYPE_H

#include "Math/BasicType.h"
#include <vector>
#include <filesystem>

enum class ETextureUsage
{
    Diffuse,  // 漫反射
    Specular, // 高光
    Normal,   // 法线
    Height,   // 高度
    Ambient,  // 环境光
};

struct Vertex
{
    Vector3 position;
    Vector3 normal;
    Vector2 texCoord;
};

struct Texture
{
    uint32_t id;
    ETextureUsage usage;
};

class Mesh
{

public:
    inline const std::vector<Vertex> &GetVertices() const {return m_vertices;}
    inline const std::vector<uint32_t> &GetIndices() const {return m_indices;}
    inline const std::vector<Texture> &GetTextures() const {return m_textures;}

    static Mesh* LoadMesh(const char *path);

private:
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    std::vector<Texture> m_textures;

    std::filesystem::path m_path;
};

#endif // ZEPHYR_GEOMETRYTYPE_H
