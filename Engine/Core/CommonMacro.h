/**
 * @file CommonMacro.h
 * @author Echo
 * @Date 24-3-8
 * @brief
 */

#ifndef COMMONMACRO_H
#define COMMONMACRO_H

#undef interface
#define interface class
#define OUT

#define GENERATED_BODY(...)                                                                                            \
public:                                                                                                                \
    Object *Copy() const override;                                                                                           \
                                                                                                                       \
private:                                                                                                               \

// 是否是MSVC编译器
#ifndef REFLECTION
#define PROPERTY()
#else
#define PROPERTY() __attribute__((annotate("property-reflect")))
#endif

#endif // COMMONMACRO_H
