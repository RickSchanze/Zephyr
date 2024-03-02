/**
 * @file RenderBufferObject.h
 * @author Echo
 * @Date 2024/3/2
 * @brief
 */

#ifndef ZEPHYR_RENDER_BUFFER_OBJECT_H
#define ZEPHYR_RENDER_BUFFER_OBJECT_H

#include <cstdint>

#ifndef __glad_h_
#include "glad/glad.h"
#endif

namespace Platform::GL
{

class RenderBufferObject
{
public:
    RenderBufferObject();
    ~RenderBufferObject();
    /** 获取id */
    inline uint32_t GetId() const;
    /** 绑定RenderBuffer */
    inline void Bind() const;

    /**
     * 为RenderBuffer分配内存
     * @note 使用前请先Bind
     * @param target 目标
     * @param internalFormat 内部格式
     * @param width
     * @param height
     */
    void AllocateStorage(int32_t width, int32_t height, uint32_t target = GL_RENDERBUFFER, uint32_t internalFormat = GL_DEPTH24_STENCIL8);

private:
    uint32_t m_id = 0;
    int32_t m_width = 0;
    int32_t m_height = 0;
};

} // namespace Platform::GL

#endif // ZEPHYR_RENDER_BUFFER_OBJECT_H
