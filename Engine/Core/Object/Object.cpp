/**
 * @file Object.cpp
 * @author Echo 
 * @Date 24-3-25
 * @brief 
 */

#include "Object.h"

void Object::SetClassDefaultObject(const Reflection::Class *class_, Object *obj)
{
    s_class_default_objects[class_->GetName()] = obj;
}

Object *Object::GetClassDefaultObject(const Reflection::Class *class_)
{
    if (!s_class_default_objects.contains(class_->GetName()))
        return nullptr;
    return s_class_default_objects[class_->GetName()];
}

Object *Object::Copy() const
{
    if (m_class != nullptr)
    {
        const size_t obj_size = m_class->GetSize();
        void* obj = malloc(obj_size);
        memcpy(obj, this, obj_size);
        return static_cast<Object*>(obj);
    }
    return nullptr;
}
