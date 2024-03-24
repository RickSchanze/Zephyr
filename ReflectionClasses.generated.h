#pragma once
#include "Reflection/MetaType.h"
class A;

namespace Reflection {
namespace Detail{

template <>
Class *GetClassImpl(ClassTag<A>) noexcept {
static ClassBuilder<A, 5, 0> class_builder([](auto* builder) {
builder->fields[0] = Field::MakeField<int32_t>("a1", offsetof(A, a1));
builder->fields[1] = Field::MakeField<int8_t>("a2", offsetof(A, a2));
builder->fields[2] = Field::MakeField<int16_t>("a3", offsetof(A, a3));
builder->fields[3] = Field::MakeField<float>("a4", offsetof(A, a4));
builder->fields[4] = Field::MakeField<double>("a5", offsetof(A, a5));
});
static Class cache(nullptr, class_builder.fields, class_builder.fields + class_builder.num_fields, "A", sizeof(A));
class_builder.SetFieldsOwner(&cache);
return &cache;
}

template <>
Type *GetTypeImpl(TypeTag<A>) noexcept {
return GetClassImpl(ClassTag<A>{});
}

}
}

class A;
class B;

namespace Reflection {
namespace Detail{

template <>
Class *GetClassImpl(ClassTag<B>) noexcept {
static ClassBuilder<B, 5, 0> class_builder([](auto* builder) {
builder->fields[0] = Field::MakeField<int32_t>("b1", offsetof(B, b1));
builder->fields[1] = Field::MakeField<int8_t>("b2", offsetof(B, b2));
builder->fields[2] = Field::MakeField<int16_t>("b3", offsetof(B, b3));
builder->fields[3] = Field::MakeField<float>("b4", offsetof(B, b4));
builder->fields[4] = Field::MakeField<double>("b5", offsetof(B, b5));
});
static Class cache(GetClass<A>(), class_builder.fields, class_builder.fields + class_builder.num_fields, "B", sizeof(B));
class_builder.SetFieldsOwner(&cache);
return &cache;
}

template <>
Type *GetTypeImpl(TypeTag<B>) noexcept {
return GetClassImpl(ClassTag<B>{});
}

}
}

class B;
class C;

namespace Reflection {
namespace Detail{

template <>
Class *GetClassImpl(ClassTag<C>) noexcept {
static ClassBuilder<C, 5, 0> class_builder([](auto* builder) {
builder->fields[0] = Field::MakeField<int32_t>("c1", offsetof(C, c1));
builder->fields[1] = Field::MakeField<int8_t>("c2", offsetof(C, c2));
builder->fields[2] = Field::MakeField<int16_t>("c3", offsetof(C, c3));
builder->fields[3] = Field::MakeField<float>("c4", offsetof(C, c4));
builder->fields[4] = Field::MakeField<B *>("c5", offsetof(C, c5));
});
static Class cache(GetClass<B>(), class_builder.fields, class_builder.fields + class_builder.num_fields, "C", sizeof(C));
class_builder.SetFieldsOwner(&cache);
return &cache;
}

template <>
Type *GetTypeImpl(TypeTag<C>) noexcept {
return GetClassImpl(ClassTag<C>{});
}

}
}

class A;
class D;

namespace Reflection {
namespace Detail{

template <>
Class *GetClassImpl(ClassTag<D>) noexcept {
static ClassBuilder<D, 5, 0> class_builder([](auto* builder) {
builder->fields[0] = Field::MakeField<std::vector<int32_t *>>("d1", offsetof(D, d1));
builder->fields[1] = Field::MakeField<std::vector<int32_t>>("d2", offsetof(D, d2));
builder->fields[2] = Field::MakeField<std::string>("d3", offsetof(D, d3));
builder->fields[3] = Field::MakeField<C>("d4", offsetof(D, d4));
builder->fields[4] = Field::MakeField<std::vector<std::string *>>("d5", offsetof(D, d5));
});
static Class cache(GetClass<A>(), class_builder.fields, class_builder.fields + class_builder.num_fields, "D", sizeof(D));
class_builder.SetFieldsOwner(&cache);
return &cache;
}

template <>
Type *GetTypeImpl(TypeTag<D>) noexcept {
return GetClassImpl(ClassTag<D>{});
}

}
}

