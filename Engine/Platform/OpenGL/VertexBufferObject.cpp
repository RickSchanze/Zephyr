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

void VertexBufferObject::SetData(const void *data, uint32_t size, uint32_t usage, uint32_t target)
{
    glBufferData(target, size, data, usage);
}

VertexBufferObject& VertexBufferObject::BindVertexAttributePointer(int32_t count, uint32_t data_type, bool normalize)
{
    glVertexAttribPointer(m_enabled_attribute_count, count, data_type, normalize, (int)(count * sizeof(float)), reinterpret_cast<void*>(offset));
    offset += count * sizeof(float);
    m_enabled_attribute_count++;
    return *this;
}
