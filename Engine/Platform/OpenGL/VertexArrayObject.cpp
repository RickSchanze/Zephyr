/**
 * @file VertexArrayObject.cpp
 * @author Echo
 * @Date 2024/3/2
 * @brief
 */

#include "VertexArrayObject.h"

using namespace Platform::GL;

VertexArrayObject::VertexArrayObject()
{
    glGenVertexArrays(1, &m_id);
    // TODO: 可能的错误处理
}

VertexArrayObject::~VertexArrayObject()
{
    glDeleteVertexArrays(1, &m_id);
}

uint32_t VertexArrayObject::GetId() const
{
    return m_id;
}

void VertexArrayObject::Bind() const
{
    glBindVertexArray(m_id);
}

void VertexArrayObject::Unbind() const
{
    glBindVertexArray(0);
}
