/**
 * @file Model.cpp
 * @author Echo
 * @Date 2024/3/6
 * @brief
 */

#include "Model.h"

#include "AssetManager.h"

#include <filesystem>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include "Geometry/Mesh.h"
#include "Logger/Logger.h"

namespace fs = std::filesystem;

namespace Resource {
Mesh *Model::ProcessMesh(const aiMesh *mesh, const aiScene *scene) const {
  const auto result = new Mesh();
  // 处理顶点
  result->m_vertices.resize(mesh->mNumVertices);
  for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex{};
    vertex.position.x = mesh->mVertices[i].x;
    vertex.position.y = mesh->mVertices[i].y;
    vertex.position.z = mesh->mVertices[i].z;

    vertex.normal.x = mesh->mNormals[i].x;
    vertex.normal.y = mesh->mNormals[i].y;
    vertex.normal.z = mesh->mNormals[i].z;

    // 这里只使用第一组纹理坐标
    if (mesh->mTextureCoords[0]) { // 网格是否有纹理坐标？
      vertex.tex_coords.x = mesh->mTextureCoords[0][i].x;
      vertex.tex_coords.y = mesh->mTextureCoords[0][i].y;
    } else {
      ZEPHYR_LOG_INFO("here");
      vertex.tex_coords = glm::vec2(0.0f, 0.0f);
    }
    result->m_vertices[i] = vertex;
  }

  // 处理索引
  for (size_t i = 0; i < mesh->mNumFaces; i++) {
    const aiFace face = mesh->mFaces[i];
    for (size_t j = 0; j < face.mNumIndices; j++) {
      result->m_indices.push_back(face.mIndices[j]);
    }
  }

  // 处理纹理
  const aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
  // 漫反射贴图
  std::vector<Texture> diffuse;
  LoadMaterialTextures(material, aiTextureType_DIFFUSE, ETextureUsage::Diffuse, diffuse);
  result->m_textures.insert(result->m_textures.end(), diffuse.begin(), diffuse.end());
  // 高光
  std::vector<Texture> specular;
  LoadMaterialTextures(material, aiTextureType_SPECULAR, ETextureUsage::Specular, specular);
  result->m_textures.insert(result->m_textures.end(), specular.begin(), specular.end());
  // 法线
  std::vector<Texture> normal;
  LoadMaterialTextures(material, aiTextureType_HEIGHT, ETextureUsage::Normal, normal);
  result->m_textures.insert(result->m_textures.end(), normal.begin(), normal.end());
  // 高度
  std::vector<Texture> height;
  LoadMaterialTextures(material, aiTextureType_AMBIENT, ETextureUsage::Height, height);
  result->m_textures.insert(result->m_textures.end(), height.begin(), height.end());
  return result;
}

void Model::LoadMaterialTextures(const aiMaterial *mat, const aiTextureType type, const ETextureUsage usage, OUT std::vector<Texture> &textures) const {
  // 将纹理数组调整到合适大小
  if (textures.size() < mat->GetTextureCount(type)) {
    textures.resize(mat->GetTextureCount(type));
  }
  for (size_t i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);
    std::wstring wstr;
    utf8::utf8to16(str.C_Str(), str.C_Str() + str.length, std::back_inserter(wstr));
    // TODO: 优化，集中管理，不重复加载图像
    // 加载纹理
    const auto image = AssetManager::Get().Request<Image>(m_directory + L"/" + wstr);
    textures[i] = Texture{image, usage};
  }
}

void Model::ProcessNode(const aiNode *node, const aiScene *scene) {
  // 处理所有网格
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    const aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    m_meshes.push_back(ProcessMesh(mesh, scene));
  }
  // 然后对子节点重复
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    ProcessNode(node->mChildren[i], scene);
  }
}

std::shared_ptr<Model> Model::Create(const std::wstring &path) {
  auto rtn = std::make_shared<Model>();
  rtn->m_path = path;
  rtn->Load();
  return rtn;
}

bool Model::IsValid() const {
  return m_valid;
}

Model::~Model() {
  for (const auto &mesh : m_meshes) {
    delete mesh;
  }
}

void Model::Load() {
  if (!fs::exists(m_path)) {
    ZEPHYR_LOG_ERROR("Model not found: {}", m_path);
    return;
  }

  Assimp::Importer importer;
  const std::string model_path_char{m_path.begin(), m_path.end()};
  const aiScene *scene = importer.ReadFile(model_path_char.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    ZEPHYR_LOG_ERROR("Failed to load model: {}", importer.GetErrorString());
    return;
  }
  std::string directory_path_char = fs::path(m_path).parent_path().string();
  m_directory = std::wstring{directory_path_char.begin(), directory_path_char.end()};
  ProcessNode(scene->mRootNode, scene);
  m_valid = true;
}
} // namespace Resource
