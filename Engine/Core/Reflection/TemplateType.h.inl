namespace Detail
{
/** Vector类型 手动重载 */
template <typename T>
inline Class *GetClassImpl(ClassTag<std::vector<T>>) noexcept
{
    static ClassBuilder<T, 0, 0, 1> builder([](auto self) {
        self->template_args[0] = TemplateArgument{GetType<T>()};
    });
    static ClassTemplate cache(nullptr, builder.fields, builder.fields + builder.num_fields, "std::vector",
                               sizeof(std::vector<T>), TypeFlag::IsVector, builder.template_args,
                               builder.template_args + builder.num_template_args);
    return &cache;
}

template <typename T>
inline Type* GetTypeImpl(TypeTag<std::vector<T>>) noexcept
{
    return GetClassImpl(ClassTag<std::vector<T>>{});
}

/** 重载std::string */
template <>
inline Class *GetClassImpl(ClassTag<std::string>) noexcept
{
    static ClassBuilder<char, 0, 0, 1> builder([](auto self) {
        self->template_args[0] = TemplateArgument{GetType<char>()};
    });
    static ClassTemplate cache(nullptr, builder.fields, builder.fields + builder.num_fields, "std::string",
                               sizeof(std::string), TypeFlag::IsString, builder.template_args,
                               builder.template_args + builder.num_template_args);
    return &cache;
}

template <>
inline Type* GetTypeImpl(TypeTag<std::string>) noexcept
{
    return GetClassImpl(ClassTag<std::string>{});
}

} // namespace Reflection::Detail

