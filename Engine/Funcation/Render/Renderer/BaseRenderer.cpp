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

using namespace Platform::GL;

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

void BaseRenderer::SetVertexBufferData(const void *data, const uint32_t size, const uint32_t usage, const uint32_t target)
{
    m_vao->Bind();
    m_vbo->Bind();
    m_vbo->SetData(data, size, usage, target);
    m_vao->Unbind();
}

void BaseRenderer::SetElementBufferData(const void *indices_data, const int32_t size, const int32_t type_size, const uint32_t usage)
{
    m_vao->Bind();
    m_ebo->Bind();
    m_ebo->SetData(indices_data, size, usage);
    m_vao->Unbind();
    m_vertex_count = size / type_size;
}

BaseRenderer &BaseRenderer::BindVertexAttributePointer(const int32_t count, uint32_t offset, const uint32_t data_type, const bool normalize)
{
    m_vao->Bind();
    m_vbo->Bind();
    m_vbo->BindVertexAttributePointer(count, offset, data_type, normalize);
    m_vao->Unbind();
    return *this;
}
