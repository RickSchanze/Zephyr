/**
 * @file Asset.h
 * @author Echo
 * @Date 24-3-8
 * @brief 用于定义一个资产应该具有的基本属性
 */

#ifndef ASSET_H
#define ASSET_H

#include "CommonMacro.h"

#include <string>

interface IAsset {
public:
  IAsset() = default;
  virtual ~IAsset() = default;
  /** 获取这个资产的路径 */
  virtual std::wstring GetPath() = 0;
  /** 设置资产路径 */
  virtual void SetPath(const std::wstring& path) = 0;
  /** 从路径中加载资源 */
  virtual void Load() = 0;
  /** 验证此资源是否有效 */
  virtual bool IsValid() = 0;
};

#endif //ASSET_H
