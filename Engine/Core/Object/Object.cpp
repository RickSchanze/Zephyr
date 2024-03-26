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
