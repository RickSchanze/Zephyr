/**
 * @file FrameBufferObject.cpp
 * @author Echo
 * @Date 2024/3/2
 * @brief
 */

#include "FrameBufferObject.h"

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

} // namespace Platform::GL
