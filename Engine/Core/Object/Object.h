/**
 * @file Object.h
 * @author Echo
 * @Date 24-3-25
 * @brief
 */

#ifndef OBJECT_H
#define OBJECT_H
#include "Reflection/MetaType.h"

#include <string>
#include <unordered_map>

class Object
{
public:
    static void SetClassDefaultObject(const Reflection::Class *class_, Object *obj);
    static Object *GetClassDefaultObject(const Reflection::Class *class_);
    Object() = default;

    virtual ~Object() = default;

    /**
     * 拷贝此对象 内部使用memcpy
     * @return
     */
    virtual Object *Copy() const = 0;

    /**
     * 设置这个对象的class 不应调用此函数 仅供Reflection调用
     * @param class_
     */
    void InternalSetClass(Reflection::Class* class_) {m_class = class_;}

private:
    // 类默认对象
    inline static std::unordered_map<std::string, Object *> s_class_default_objects;
    Reflection::Class *m_class = nullptr;
};

inline Object *New(const Reflection::Class *r_class)
{
    const Object* cdo = Object::GetClassDefaultObject(r_class);
    if (cdo == nullptr) return nullptr;
    return cdo->Copy();
}

template <typename R>
    requires std::is_base_of_v<Object, R>
R *New()
{
    return static_cast<R*>(New(Reflection::GetClass<R>()));
}

#endif //OBJECT_H
