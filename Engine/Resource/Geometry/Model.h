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

namespace Resource {

class Mesh;

class Model {
public:
  void Load(const std::string &path);
  static Model *Create(const std::string &path);

protected:
  void ProcessNode(aiNode *node, const aiScene *scene);
  Mesh *ProcessMesh(aiMesh *mesh, const aiScene *scene);

private:
  std::string m_path;
  std::string m_directory;
  std::vector<Mesh *> m_meshes;
};
} // namespace Resource

#endif // ZEPHYR_MODEL_H
