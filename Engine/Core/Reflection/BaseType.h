#ifndef BASETYPE_H
#define BASETYPE_H
namespace Detail
{

#define DELCARE_BASE_TYPE_IMPL(type)                                                                                   \
template <>                                                                                                        \
inline Type *GetTypeImpl(TypeTag<type>) noexcept                                                                   \
{                                                                                                                  \
static Type my_type(#type, sizeof(type));                                                                         \
return &my_type;                                                                                                  \
}

DELCARE_BASE_TYPE_IMPL(int8_t)
DELCARE_BASE_TYPE_IMPL(int16_t)
DELCARE_BASE_TYPE_IMPL(int32_t)
DELCARE_BASE_TYPE_IMPL(int64_t)
DELCARE_BASE_TYPE_IMPL(uint8_t)
DELCARE_BASE_TYPE_IMPL(uint16_t)
DELCARE_BASE_TYPE_IMPL(uint32_t)
DELCARE_BASE_TYPE_IMPL(uint64_t)
DELCARE_BASE_TYPE_IMPL(float)
DELCARE_BASE_TYPE_IMPL(double)

// 对String的特殊重载
template <>
inline Type *GetTypeImpl(TypeTag<std::string>) noexcept
{
    static Type my_type("std::string", sizeof(std::string));
    return &my_type;
}

} // namespace Detail

#endif
