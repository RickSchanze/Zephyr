#ifndef BASETYPE_H
#define BASETYPE_H
#include <vector>
namespace Detail
{

#define DELCARE_BASE_TYPE_IMPL(type)                                                                                   \
template <>                                                                                                        \
inline Type *GetTypeImpl(TypeTag<type>) noexcept                                                                   \
{                                                                                                                  \
static Type my_type(#type, sizeof(type));                                                                         \
return &my_type;                                                                                                  \
}

DELCARE_BASE_TYPE_IMPL(int)
DELCARE_BASE_TYPE_IMPL(char)
DELCARE_BASE_TYPE_IMPL(short)
DELCARE_BASE_TYPE_IMPL(float)
DELCARE_BASE_TYPE_IMPL(double)

} // namespace Detail

#endif
