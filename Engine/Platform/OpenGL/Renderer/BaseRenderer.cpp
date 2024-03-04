/**
 * @file BaseRenderer.cpp
 * @author Echo
 * @Date 2024/3/4
 * @brief
 */

#include "BaseRenderer.h"

#include "OpenGL/ElementBufferObject.h"
#include "OpenGL/VertexArrayObject.h"
#include "OpenGL/VertexBufferObject.h"

namespace Platform::GL
{
BaseRenderer::BaseRenderer()
{
    m_vao = new VertexArrayObject();
    m_vbo = new VertexBufferObject();
    m_ebo = new ElementBufferObject();
}

BaseRenderer::~BaseRenderer()
{
    delete m_vao;
    delete m_vbo;
    delete m_ebo;
}

void BaseRenderer::SetVertexBufferData(const void *data, uint32_t size, uint32_t usage, uint32_t target)
{
    m_vao->Bind();
    m_vbo->Bind();
    m_vbo->SetData(data, size, usage, target);
    m_vao->Unbind();
}

void BaseRenderer::SetElementBufferData(const void *indices_data, int32_t size, int32_t type_size, uint32_t usage)
{
    m_vao->Bind();
    m_ebo->Bind();
    m_ebo->SetData(indices_data, size, usage);
    m_vao->Unbind();
    m_vertex_count = size / type_size;
}

BaseRenderer &BaseRenderer::BindVertexAttributePointer(int32_t count, uint32_t data_type, bool normalize)
{
    m_vao->Bind();
    m_vbo->Bind();
    m_vbo->BindVertexAttributePointer(count, data_type, normalize);
    m_vao->Unbind();
    return *this;
}

} // namespace Platform::GL
