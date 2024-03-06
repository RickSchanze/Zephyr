/**
 * @file Model.cpp
 * @author Echo
 * @Date 2024/3/6
 * @brief
 */

#include "Model.h"

#include <filesystem>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include "Geometry/Mesh.h"
#include "Logger/Logger.h"

namespace fs = std::filesystem;

namespace Resource {
Mesh *Model::ProcessMesh(aiMesh *mesh, const aiScene *scene) {
  Mesh *result = new Mesh();
  for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex{};
    vertex.position.x = mesh->mVertices[i].x;
    vertex.position.y = mesh->mVertices[i].y;
    vertex.position.z = mesh->mVertices[i].z;

    vertex.normal.x = mesh->mNormals[i].x;
    vertex.normal.y = mesh->mNormals[i].y;
    vertex.normal.z = mesh->mNormals[i].z;

    if (mesh->mTextureCoords[0]) {
      // TODO: 填充剩余的纹理坐标
    }
  }
}

void Model::ProcessNode(aiNode *node, const aiScene *scene) {
  // 处理所有网格
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    m_meshes.push_back(ProcessMesh(mesh, scene));
  }
  // 然后对子节点重复
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    ProcessNode(node->mChildren[i], scene);
  }
}

Model *Model::Create(const std::string &path) {
  return nullptr;
}

void Model::Load(const std::string &path) {
  if (!fs::exists(path)) {
    ZEPHYR_LOG_ERROR("Model not found: {}", path);
    return;
  }
  m_path = path;
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    ZEPHYR_LOG_ERROR("Failed to load model: {}", importer.GetErrorString());
    return;
  }
  m_directory = fs::path(path).parent_path().string();
  ProcessNode(scene->mRootNode, scene);
}
} // namespace Resource
