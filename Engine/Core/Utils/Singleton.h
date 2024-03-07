/**
 * @file Singleton.h
 * @author Echo
 * @Date 24-3-8
 * @brief 单例模板实现
 */

#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton {
public:
  static T &Get() {
    static T instance;
    return instance;
  }

  explicit Singleton(const T &) = delete;
  T& operator=(const T &) = delete;

protected:
  Singleton() = default;
  virtual ~Singleton() = default;
};

#endif // SINGLETON_H
