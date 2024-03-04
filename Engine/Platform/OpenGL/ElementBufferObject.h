/**
 * @file ElementBufferObject.h
 * @author Echo
 * @Date 2024/3/2
 * @brief
 */

#ifndef ZEPHYR_ELEMENTBUFFEROBJECT_H
#define ZEPHYR_ELEMENTBUFFEROBJECT_H
#include <cstdint>

#ifndef __glad_h_
#include "glad/glad.h"
#endif

namespace Platform::GL
{

class ElementBufferObject
{
public:
    ElementBufferObject();
    ~ElementBufferObject();

    /** 获取ebo id */
    inline uint32_t GetId() const;
    /** 绑定EBO */
    void Bind() const;
    /** 解绑 */
    void Unbind() const;

    /**
     * @brief 设置EBO数据
     * @param indices_data 索引数据指针
     * @param size 索引数据大小
     * @param usage 数据用途 默认GL_STATIC_DRAW
     */
    void SetData(const void *indices_data, uint32_t size, uint32_t usage = GL_STATIC_DRAW);

private:
    uint32_t m_id{0};
};

} // namespace Platform::GL

#endif // ZEPHYR_ELEMENTBUFFEROBJECT_H
