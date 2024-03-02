/**
 * @file Texture.cpp
 * @author Echo
 * @Date 2024/3/2
 * @brief
 */

#include "Texture.h"
#include "Logger/Logger.h"

namespace Platform::GL
{

Texture::Texture()
{
    glGenTextures(1, &m_id);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}
uint32_t Texture::GetId() const
{
    return m_id;
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}

Texture &Texture::SetImageParam(const TextureParam &param)
{
    if (param.width <= 0 || param.height <= 0)
    {
        g_logger.Error("Texture::SetImageParam: width or height is invalid");
        return *this;
    }
    glTexImage2D(
        param.target,
        param.level,
        param.internalFormat,
        param.width,
        param.height,
        param.border,
        param.format,
        param.type,
        param.data);
    m_param = param;
    return *this;
}

Texture &Texture::SetParam(const int32_t pname, const int32_t param, const uint32_t target)
{
    glTexParameteri(target, pname, param);
    return *this;
}

Texture &Texture::Initialize(const int32_t width, const int32_t height)
{
    Bind();
    SetImageParam({.width = width, .height = height})
        .SetParam(GL_TEXTURE_MIN_FILTER, GL_LINEAR)
        .SetParam(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return *this;
}

} // namespace Platform::GL
