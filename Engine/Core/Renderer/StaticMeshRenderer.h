/**
 * @file StaticMeshRenderer.h
 * @author Echo
 * @Date 2024/3/4
 * @brief
 */

#ifndef ZEPHYR_STATICMESHRENDERER_H
#define ZEPHYR_STATICMESHRENDERER_H

#include "BaseRenderer.h"
#include "OpenGL/Shader.h"

namespace Platform::GL
{
class ShaderProgram;
}

class StaticMeshRenderer : public BaseRenderer
{
    typedef Platform::GL::ShaderProgram ShaderProgram;
    typedef Platform::GL::VertexShader VertexShader;
    typedef Platform::GL::FragmentShader FragmentShader;

public:
    /**
     * 构造函数
     * @param vertex_shader_path 顶点着色器路径
     * @param fragment_shader_path 片元着色器路径
     */
    StaticMeshRenderer(const char* vertex_shader_path, const char* fragment_shader_path);

    void Draw() override;

    ~StaticMeshRenderer() override;

protected:
    VertexShader* m_vertex_shader;
    FragmentShader* m_fragment_shader;
    ShaderProgram* m_shader_program{};
    bool m_initialized{false};
};

#endif // ZEPHYR_STATICMESHRENDERER_H
