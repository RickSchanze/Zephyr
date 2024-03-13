/**
 * @file RenderComponentBase.h
 * @author Echo 
 * @Date 24-3-7
 * @brief 
 */

#ifndef RENDERCOMPONENTBASE_H
#define RENDERCOMPONENTBASE_H
#include "ComponentBase.h"

/**
 * 渲染组件的基类
 */
class RendererComponentBase : public ComponentBase {
public:
  RendererComponentBase() = default;
  ~RendererComponentBase() override = default;

  virtual void Initialize() = 0;
  virtual void TickRender() = 0;
  virtual void Deinitialize() = 0;
};



#endif //RENDERCOMPONENTBASE_H
