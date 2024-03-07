/**
 * @file ViewportWindow.h
 * @author Echo
 * @Date 24-3-3
 * @brief
 */

#ifndef VIEWPORTWINDOW_H
#define VIEWPORTWINDOW_H
#include "BaseWindow.h"

class StaticModelRendererComponent;
class FrameRenderer;

class ViewportWindow : public BaseWindow {
public:
  ViewportWindow();
  ~ViewportWindow();

  void Draw() override;

  void Resize(int width, int height);

private:
  FrameRenderer *m_frame_render{};
  int m_width{};
  int m_height{};
};

#endif // VIEWPORTWINDOW_H
