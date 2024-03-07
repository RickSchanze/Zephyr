/**
 * @file AssetBase.h
 * @author Echo
 * @Date 24-3-8
 * @brief
 */

#ifndef ASSETBASE_H
#define ASSETBASE_H

#include <utility>

#include "Interface/Asset.h"

class AssetBase : public IAsset {
public:
  AssetBase() : AssetBase(L"") {}
  explicit AssetBase(std::wstring path) : m_path(std::move(path)) {}
  ~AssetBase() override = default;

  std::wstring GetPath() override {
    return m_path;
  }

  /** 设置资产路径 */
  void SetPath(const std::wstring &path) override {
    m_path = path;
  }

  /** 资产是否有效 */
  bool IsValid() override {
    return m_valid;
  }

protected:
  std::wstring m_path;
  bool m_valid = false;
};

#endif // ASSETBASE_H
