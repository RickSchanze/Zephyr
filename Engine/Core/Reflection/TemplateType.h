#pragma once
#include "MetaType.h"

namespace Reflection::Detail
{
/** Vector类型 手动重载 */
template <typename T>
Class *GetClassImpl(ClassTag<std::vector<T>>) noexcept
{
    static ClassBuilder<T, 0, 0, 1> builder([](auto self) { self->template_args[0] = GetType<T>(); });
    static ClassTemplate cache(nullptr, builder.fields, builder.fields + builder.num_fields, "std::vector<T>",
                               sizeof(std::vector<T>), builder.template_args, TypeFlag::IsVector,
                               builder.template_args + builder.num_template_args);
    return &cache;
}
}

