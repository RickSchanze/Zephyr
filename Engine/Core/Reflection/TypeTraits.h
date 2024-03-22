#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H
#include <string>
#include <vector>

// 判断一个类型是不是Vector变量
template <typename T>
struct IsStdVector
{
    static constexpr bool value = false;
};

template <typename T>
struct IsStdVector<std::vector<T>>
{
    static constexpr bool value = true;
};

// 提取std vector的模板类型
template <typename T>
struct ExtractStdVectorType
{
    using type = T;
};

template <typename T>
struct ExtractStdVectorType<std::vector<T>>
{
    using type = T;
};

// 判断一个类型是不是string变量
template <typename T>
struct IsStdString
{
    static constexpr bool value = false;
};


template <>
struct IsStdString<std::string>
{
    static constexpr bool value = true;
};

// 提取string的模板类型
template <typename T>
struct ExtractStdStringType
{
    using type = T;
};

#endif
