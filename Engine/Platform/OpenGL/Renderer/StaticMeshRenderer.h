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

class StaticMeshRenderer : public BaseRenderer
{
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
    bool m_initialized{false};
    uint32_t m_program_id{};
};

} // namespace Platform::GL


#endif // ZEPHYR_STATICMESHRENDERER_H
