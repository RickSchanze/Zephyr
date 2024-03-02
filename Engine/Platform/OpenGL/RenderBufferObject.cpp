/**
 * @file RenderBufferObject.cpp
 * @author Echo
 * @Date 2024/3/2
 * @brief
 */

#include "RenderBufferObject.h"
#include "Logger/Logger.h"

namespace Platform::GL
{

uint32_t RenderBufferObject::GetId() const
{
    return m_id;
}

RenderBufferObject::RenderBufferObject()
{
    glGenRenderbuffers(1, &m_id);
}

RenderBufferObject::~RenderBufferObject()
{
    glDeleteRenderbuffers(1, &m_id);
}

void RenderBufferObject::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_id);
}

void RenderBufferObject::AllocateStorage(const int32_t width, const int32_t height, const uint32_t target, const uint32_t internalFormat)
{
    if (width <= 0 || width >= GL_MAX_RENDERBUFFER_SIZE)
    {
        ZEPHYR_LOG_ERROR("width is invalid.");
        return;
    }

    if (height <= 0 || height >= GL_MAX_RENDERBUFFER_SIZE)
    {
        ZEPHYR_LOG_ERROR("height is invalid");
        return;
    }

    glRenderbufferStorage(target, internalFormat, width, height);
}

} // namespace Platform::GL
