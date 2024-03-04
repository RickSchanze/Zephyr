/**
 * @file StaticMeshRenderer.cpp
 * @author Echo
 * @Date 2024/3/4
 * @brief
 */

#include "StaticMeshRenderer.h"
#include "OpenGL/VertexArrayObject.h"


namespace Platform::GL
{

StaticMeshRenderer::StaticMeshRenderer(const char *vertex_shader_path, const char *fragment_shader_path)
{
    m_vertex_shader = new VertexShader(vertex_shader_path);
    m_fragment_shader = new FragmentShader(fragment_shader_path);
}

StaticMeshRenderer::~StaticMeshRenderer()
{
    delete m_vertex_shader;
    delete m_fragment_shader;
}

void StaticMeshRenderer::Draw()
{
    if (!m_initialized)
    {
        m_vertex_shader->Initialize();
        m_fragment_shader->Initialize();
        m_program_id = glCreateProgram();
        glAttachShader(m_program_id, m_vertex_shader->GetId());
        glAttachShader(m_program_id, m_fragment_shader->GetId());
        glLinkProgram(m_program_id);
        m_initialized = true;
    }

    glUseProgram(m_program_id);
    m_vao->Bind();
    glDrawElements(GL_TRIANGLES, m_vertex_count, GL_UNSIGNED_INT, nullptr);

    auto a = glGetError();

}

} // namespace Platform::GL

