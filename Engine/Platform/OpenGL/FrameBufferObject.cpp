/**
 * @file FrameBufferObject.cpp
 * @author Echo
 * @Date 2024/3/2
 * @brief
 */

#include "FrameBufferObject.h"

#include "Logger/Logger.h"
#include "RenderBufferObject.h"
#include "Texture.h"

namespace Platform::GL
{

FrameBufferObject::FrameBufferObject()
{
    glGenFramebuffers(1, &m_id);
}

FrameBufferObject::~FrameBufferObject()
{
    glDeleteFramebuffers(1, &m_id);
}

uint32_t FrameBufferObject::GetId() const
{
    return m_id;
}

void FrameBufferObject::AttachTexture(const Texture &texture, const uint32_t attachment)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.GetId(), texture.GetParam().level);
}

void FrameBufferObject::AttachRenderBuffer(const RenderBufferObject &render_buffer, const uint32_t attachment)
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, render_buffer.GetId());
}

void FrameBufferObject::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void FrameBufferObject::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool FrameBufferObject::Check()
{
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        ZEPHYR_LOG_ERROR("Framebuffer not complete!");
        return false;
    }
    return true;
}

} // namespace Platform::GL
