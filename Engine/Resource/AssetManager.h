/**
 * @file AssetManager.h
 * @author Echo
 * @Date 24-3-8
 * @brief 资产管理器
 */

#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "Logger/Logger.h"
#include "Utils/Singleton.h"

#include <memory>
#include <string>
#include <unordered_map>


class IAsset;
class AssetManager : public Singleton<AssetManager> {
public:
  /** 同步请求一个资产 */
  template <typename T> requires std::derived_from<T, IAsset>
  std::shared_ptr<T> Request(const std::wstring& path) {
    if (m_assets.contains(path)) {
      ZEPHYR_LOG_INFO("AssetManager: Load cached Asset: {}", path);
      return std::dynamic_pointer_cast<T>(m_assets.at(path));
    }
    auto asset = std::make_shared<T>();
    asset->SetPath(path);
    asset->Load();
    if (asset->IsValid()) {
      m_assets[path] = asset;
      ZEPHYR_LOG_INFO("AssetManager: Load new Asset: {}", asset->GetPath());
      return asset;
    }
    return nullptr;
  }

private:
  // 暂时先使用路径映射
  std::unordered_map<std::wstring, std::shared_ptr<IAsset>> m_assets;
};

#endif //ASSETMANAGER_H
