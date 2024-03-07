#include "Editor/Application.h"

int main() {
  system("chcp 65001");           // 使用 65001 代码页（UTF-8）
  Application app{1920, 1080};
  app.Run();
  return 0;
}
