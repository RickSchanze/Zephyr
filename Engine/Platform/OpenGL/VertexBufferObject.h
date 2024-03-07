/**
 * @file VertexBufferObject.h
 * @author Echo
 * @Date 2024/3/2
 * @brief OpenGL VBO封装
 */

#ifndef ZEPHYR_VERTEXBUFFEROBJECT_H
#define ZEPHYR_VERTEXBUFFEROBJECT_H

#include <cstdint>

#ifndef __glad_h_
#include "glad/glad.h"
#endif

namespace Platform::GL
{

class VertexBufferObject
{
public:
    VertexBufferObject();
    ~VertexBufferObject();

    /** 获取OpenGL生成的VBO */
    inline uint32_t GetId() const;
    /** 绑定当前VBO */
    void Bind() const;
    /** 解绑当前VBO */
    void Unbind() const;

    /**
     * @brief 设置VBO数据
     * @param data 数据指针
     * @param size 数据大小
     * @param usage 数据用途 默认GL_STATIC_DRAW
     * @param target 数据目标 默认GL_ARRAY_BUFFER
     */
    void SetData(const void *data, uint32_t size, uint32_t usage = GL_STATIC_DRAW, uint32_t target = GL_ARRAY_BUFFER);

    /**
     * @brief 绑定顶点属性指针，用于告诉OpenGL如何解析定点数据
     * @param count 顶点属性数量
     * @param data_type 顶点属性数据类型 GL_FLOAT...
     * @param offset 数据偏移
     * @param normalize 是否归一化 若为true则数据或被归一化至0到1之间
     * @return 返回自己(可链式调用)
     */
    VertexBufferObject &BindVertexAttributePointer(int32_t count, uint32_t offset,uint32_t data_type = GL_FLOAT,bool normalize = false);

private:
    uint32_t m_id{0};                      // VBO ID
    uint32_t m_enabled_attribute_count{0}; // 由glEnableVertexAttribArray启用的属性数量
};

} // namespace Platform::GL

#endif // ZEPHYR_VERTEXBUFFEROBJECT_H
