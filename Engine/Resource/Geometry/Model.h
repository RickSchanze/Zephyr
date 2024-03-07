/**
 * @file Model.h
 * @author Echo
 * @Date 2024/3/6
 * @brief
 */

#ifndef ZEPHYR_MODEL_H
#define ZEPHYR_MODEL_H
#include <string>

#include "assimp/scene.h"

#include "Mesh.h"

#ifndef OUT
#define OUT
#endif

namespace Resource {

class Mesh;

class Model {
public:
  ~Model();

  void Load();
  static std::shared_ptr<Model> Create(const std::wstring &path);

  /** 此模型是否有效 */
  bool IsValid() const;
  const std::vector<Mesh *> &GetMeshes() const {return m_meshes;}

protected:
  void ProcessNode(const aiNode *node, const aiScene *scene);
  Mesh *ProcessMesh(const aiMesh *mesh, const aiScene *scene) const;

  void LoadMaterialTextures(const aiMaterial *mat, aiTextureType type, ETextureUsage usage, OUT std::vector<Texture> &textures) const;

private:
  std::wstring m_path;
  std::wstring m_directory;
  std::vector<Mesh *> m_meshes;
  bool m_valid = false;
};
} // namespace Resource

#endif // ZEPHYR_MODEL_H
