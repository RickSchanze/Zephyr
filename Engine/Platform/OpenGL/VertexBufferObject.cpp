/**
 * @file VertexBufferObject.cpp
 * @author Echo
 * @Date 2024/3/2
 * @brief
 */

#include "VertexBufferObject.h"

#include "glad/glad.h"

using namespace Platform::GL;

uint32_t VertexBufferObject::GetId() const
{
    return m_id;
}

void VertexBufferObject::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferObject::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

VertexBufferObject::VertexBufferObject()
{
    glGenBuffers(1, &m_id);
    // TODO: 异常处理
}

VertexBufferObject::~VertexBufferObject()
{
    glDeleteBuffers(1, &m_id);
}

void VertexBufferObject::SetData(const void *data, const uint32_t size, const uint32_t usage, const uint32_t target)
{
    glBufferData(target, size, data, usage);
}

VertexBufferObject &VertexBufferObject::BindVertexAttributePointer(const int32_t count, const uint32_t data_type, const bool normalize)
{
    glVertexAttribPointer(m_enabled_attribute_count, count, data_type, normalize, static_cast<int>(count * sizeof(float)), reinterpret_cast<void *>(m_offset));
    glEnableVertexAttribArray(m_enabled_attribute_count);
    m_offset += count * sizeof(float);
    m_enabled_attribute_count++;
    return *this;
}
