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
class ShaderProgram;
class VertexArrayObject;
class VertexBufferObject;
class ElementBufferObject;
}

class BaseRenderer
{
public:
    typedef Platform::GL::VertexArrayObject VertexArrayObject;
    typedef Platform::GL::VertexBufferObject VertexBufferObject;
    typedef Platform::GL::ElementBufferObject ElementBufferObject;

    BaseRenderer();
    virtual ~BaseRenderer();

    /** 这里应该绑定vao后调用glDrawElement */
    virtual void Draw(const Platform::GL::ShaderProgram& shader) = 0;

    /**
     * 绑定VBO数据
     * @param data 数据指针
     * @param size 数据大小
     * @param usage 用法 默认 GL_STATIC_DRAW
     * @param target 数据目标 默认 GL_ARRAY_BUFFER
     */
    void SetVertexBufferData(const void *data, uint32_t size, uint32_t usage = GL_STATIC_DRAW, uint32_t target = GL_ARRAY_BUFFER);
    /**
     * 绑定EBO数据
     * @param indices_data 索引数据指针
     * @param size 索引数据大小
     * @param type_size 数据类型大小 默认 sizeof(unsigned int)
     * @param usage 数据用途 默认 GL_STATIC_DRAW
     */
    void SetElementBufferData(const void *indices_data, int32_t size, int32_t type_size = sizeof(unsigned int), uint32_t usage = GL_STATIC_DRAW);
    /** 绑定顶点属性指针 */
    BaseRenderer &BindVertexAttributePointer(int32_t count, uint32_t offset, uint32_t data_type = GL_FLOAT, bool normalize = false);

protected:
    VertexArrayObject *m_vao{};   // vao
    VertexBufferObject *m_vbo{};  // vbo
    ElementBufferObject *m_ebo{}; // ebo
    int32_t m_vertex_count{0};    // 顶点数量
};

#endif // ZEPHYR_BASERENDERER_H
