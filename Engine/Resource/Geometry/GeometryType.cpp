/**
 * @file GeometryType.cpp
 * @author Echo
 * @Date 2024/3/6
 * @brief
 */

#include "GeometryType.h"

#include <filesystem>

namespace fs = std::filesystem;

Mesh *Mesh::LoadMesh(const char *path)
{
    auto mesh_path = fs::path(path);
    if (!fs::exists(mesh_path))
    {
        return nullptr;
    }

    Mesh* mesh = new Mesh();
    mesh->m_path = std::move(mesh_path);

    return mesh;
}
