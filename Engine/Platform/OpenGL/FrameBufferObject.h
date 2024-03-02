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

class FrameBufferObject
{
public:
    FrameBufferObject();
    ~FrameBufferObject();
    /** 获取id */
    inline uint32_t GetId() const;

private:
    uint32_t m_id = 0;
};

} // namespace Platform::GL

#endif // ZEPHYR_FRAME_BUFFER_OBJECT_H
