/**
 * @file FrameBufferObject.h
 * @author Echo
 * @Date 2024/3/2
 * @brief
 */

#ifndef ZEPHYR_FRAME_BUFFER_OBJECT_H
#define ZEPHYR_FRAME_BUFFER_OBJECT_H

#include <cstdint>

#ifndef __glad_h_
#include "glad/glad.h"
#endif

namespace Platform::GL
{
class RenderBufferObject;
class Texture;
}

namespace Platform::GL
{

class FrameBufferObject
{
public:
    FrameBufferObject();
    ~FrameBufferObject();
    /** 获取id */
    inline uint32_t GetId() const;
    /**
     * 附加Texture
     * @param texture 需要附加的Texture
     * @param attachment 用于指定附加点 默认GL_COLOR_ATTACHMENT0
     */
    void AttachTexture(const Texture& texture, uint32_t attachment = GL_COLOR_ATTACHMENT0);
    /**
     * 附加RenderBuffer
     * @param render_buffer 要附加的RenderBuffer
     * @param attachment 附加点 默认GL_DEPTH_STENCIL_ATTACHMENT
     */
    void AttachRenderBuffer(const RenderBufferObject& render_buffer, uint32_t attachment = GL_DEPTH_STENCIL_ATTACHMENT);
    /** 绑定 */
    void Bind() const;
    /** 解绑 */
    void Unbind() const;
    /** 检查帧缓存是否完整 */
    bool Check();

private:
    uint32_t m_id = 0;
};

} // namespace Platform::GL

#endif // ZEPHYR_FRAME_BUFFER_OBJECT_H
