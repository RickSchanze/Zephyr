/**
 * @file ComponentBase.h
 * @author Echo
 * @Date 24-3-7
 * @brief
 */

#ifndef COMPONENTBASE_H
#define COMPONENTBASE_H

#include "Object/Object.h"

/**
 * 所有组件的公共基类
 */
class ComponentBase : public Object {

public:
  ComponentBase() = default;
  ~ComponentBase() override = default;

  virtual void BeginPlay(){}
  virtual void Tick() {}
  virtual void EndPlay() {}

private:

};

#endif // COMPONENTBASE_H
