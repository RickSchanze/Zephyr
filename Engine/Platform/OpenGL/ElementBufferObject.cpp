/**
 * @file ElementBufferObject.cpp
 * @author Echo
 * @Date 2024/3/2
 * @brief
 */

#include "ElementBufferObject.h"

namespace Platform::GL
{

ElementBufferObject::ElementBufferObject()
{
    glGenBuffers(1, &m_id);
}

ElementBufferObject::~ElementBufferObject()
{
    glDeleteBuffers(1, &m_id);
}

uint32_t ElementBufferObject::GetId() const
{
    return m_id;
}
void ElementBufferObject::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void ElementBufferObject::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ElementBufferObject::SetData(const void *indices_data, uint32_t size, uint32_t usage)
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices_data, usage);
}

} // namespace Platform::GL
