/**
 * @file StaticMeshRenderer.cpp
 * @author Echo
 * @Date 2024/3/4
 * @brief
 */

#include "StaticMeshRenderer.h"

#include "OpenGL/VertexArrayObject.h"
#include "OpenGL/ShaderProgram.h"

using namespace Platform::GL;

StaticMeshRenderer::StaticMeshRenderer(const char *vertex_shader_path, const char *fragment_shader_path)
{
    m_vertex_shader = new VertexShader(vertex_shader_path);
    m_fragment_shader = new FragmentShader(fragment_shader_path);
}

StaticMeshRenderer::~StaticMeshRenderer()
{
    delete m_vertex_shader;
    delete m_fragment_shader;
    delete m_shader_program;
}

void StaticMeshRenderer::Draw()
{
    if (!m_initialized)
    {
        m_vertex_shader->Initialize();
        m_fragment_shader->Initialize();
        m_shader_program = new ShaderProgram();
        m_shader_program->AttachShader(*m_vertex_shader, *m_fragment_shader);
        m_shader_program->Link();
        m_initialized = true;
    }

    m_shader_program->Use();
    m_vao->Bind();
    glDrawElements(GL_TRIANGLES, m_vertex_count, GL_UNSIGNED_INT, nullptr);

    auto a = glGetError();
}
