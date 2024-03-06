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
  void Load(const std::string &path);
  static std::shared_ptr<Model> Create(const std::string &path);

  /** 此模型是否有效 */
        bool IsValid() const;

protected:
  void ProcessNode(const aiNode *node, const aiScene *scene);
  Mesh *ProcessMesh(const aiMesh *mesh, const aiScene *scene) const;

  void LoadMaterialTextures(const aiMaterial *mat, aiTextureType type, ETextureUsage usage, OUT std::vector<Texture> &textures) const;

private:
  std::string m_path;
  std::string m_directory;
  std::vector<Mesh *> m_meshes;
  bool m_valid = false;
};
} // namespace Resource

#endif // ZEPHYR_MODEL_H
