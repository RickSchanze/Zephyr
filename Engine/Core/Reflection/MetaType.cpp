#include "MetaType.h"
#include "Object/Object.h"

namespace Reflection::Detail
{

template <>
Class *GetClassImpl(ClassTag<Object>) noexcept
{
    static ClassBuilder<Object, 0, 0> class_builder([](auto *builder) {});
    static Class cache(nullptr, class_builder.fields, class_builder.fields + class_builder.num_fields, "Object",
    sizeof(Object), [](auto* class_) {
    });
    return &cache;
}

template <>
Type *GetTypeImpl(TypeTag<Object>) noexcept
{
    return GetClassImpl(ClassTag<Object>{});
}

} // namespace Reflection::Detail
