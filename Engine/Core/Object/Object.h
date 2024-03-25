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

    virtual ~Object() = default;

    /**
     * 拷贝此对象 内部使用memcpy
     * @return
     */
    Object *Copy() const;


private:
    // 类默认对象
    inline static std::unordered_map<std::string, Object *> s_class_default_objects;
    const Reflection::Class *m_class = nullptr;
};

template <typename R>
    requires std::is_base_of_v<Object, R>
R *New()
{
    const Object *cdo = Object::GetClassDefaultObject(Reflection::GetClass<R>());
    if (cdo != nullptr)
    {
        return static_cast<R *>(cdo->Copy());
    }
    return nullptr;
}

#endif //OBJECT_H
