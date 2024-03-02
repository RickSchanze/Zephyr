/**
 * @file VertexArrayObject.h
 * @author Echo
 * @Date 2024/3/2
 * @brief
 */

#ifndef ZEPHYR_VERTEX_ARRAY_OBJECT_H
#define ZEPHYR_VERTEX_ARRAY_OBJECT_H

#include <cstdint>

#ifndef __glad_h_
#include "glad/glad.h"
#endif

namespace Platform::GL
{

class VertexArrayObject
{
public:
    VertexArrayObject();
    ~VertexArrayObject();

    /** 获取OpenGL生成的VAO */
    inline uint32_t GetId() const;
    /** 绑定当前VAO */
    void Bind() const;
    /** 解绑当前VAO */
    void Unbind() const;

private:
    uint32_t m_id{};
};

}
#endif // ZEPHYR_VERTEX_ARRAY_OBJECT_H
