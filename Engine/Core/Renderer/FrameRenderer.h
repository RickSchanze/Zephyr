/**
 * @file FrameRenderer.h
 * @author Echo
 * @Date 24-3-3
 * @brief 将图像渲染到帧缓冲区
 */

#ifndef ZEPHYR_FRAMERENDERER_H
#define ZEPHYR_FRAMERENDERER_H
#include "OpenGL/ElementBufferObject.h"
#include "OpenGL/FrameBufferObject.h"
#include "OpenGL/RenderBufferObject.h"
#include "OpenGL/Texture.h"
#include "OpenGL/VertexArrayObject.h"
#include "OpenGL/VertexBufferObject.h"
#include "StaticMeshRenderer.h"

#include <memory>

namespace Platform::GL {

class FrameBufferObject;
class RenderBufferObject;
class Texture;

}

/**
 * 帧渲染器,渲染的入口
 */
class FrameRenderer final
{
public:
    FrameRenderer() = default;

    /**
     * 初始化FrameRender
     * @param width 宽度
     * @param height 高度
     * @return 初始化是否成功
     */
    bool Initialize(int width, int height);

    /**
     * 绘制
     */
    void Draw() const;

    /**
     * 重设FrameRender大小
     * @param width 宽
     * @param height 高
     */
    void Resize(int width, int height) const;

    /** 获取绑定的Texture id */
    uint32_t GetAttachTextureId() const;

private:
    std::unique_ptr<Platform::GL::FrameBufferObject> m_fbo;
    std::unique_ptr<Platform::GL::RenderBufferObject> m_rbo;
    std::unique_ptr<Platform::GL::Texture> m_render_texture;

    std::unique_ptr<StaticMeshRenderer> renderer;
};

#endif // ZEPHYR_FRAMERENDERER_H
