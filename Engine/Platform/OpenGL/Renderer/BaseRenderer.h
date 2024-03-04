/**
 * @file BaseRenderer.h
 * @author Echo
 * @Date 2024/3/4
 * @brief
 */

#ifndef ZEPHYR_BASERENDERER_H
#define ZEPHYR_BASERENDERER_H

#include <cstdint>

#ifndef __glad_h_
#include "glad/glad.h"
#endif

namespace Platform::GL
{
class VertexArrayObject;
class VertexBufferObject;
class ElementBufferObject;

class BaseRenderer
{
public:
    BaseRenderer();
    virtual ~BaseRenderer();

    /** 这里应该绑定vao后调用glDrawElement */
    virtual void Draw() = 0;

    /** 绑定VBO数据 */
    void SetVertexBufferData(const void *data, uint32_t size, uint32_t usage = GL_STATIC_DRAW, uint32_t target = GL_ARRAY_BUFFER);
    /** 绑定EBO数据 */
    void SetElementBufferData(const void *indices_data, int32_t size, int32_t type_size = sizeof(unsigned int), uint32_t usage = GL_STATIC_DRAW);
    /** 绑定顶点属性指针 */
    BaseRenderer &BindVertexAttributePointer(int32_t count, uint32_t data_type = GL_FLOAT, bool normalize = false);

protected:
    VertexArrayObject *m_vao{};   // vao
    VertexBufferObject *m_vbo{};  // vbo
    ElementBufferObject *m_ebo{}; // ebo
    int32_t m_vertex_count{0};   // 顶点数量
};
} // namespace Platform::GL

#endif // ZEPHYR_BASERENDERER_H
