#include "Reflection/JsonDeserializer.h"
#include "Reflection/JsonSerializer.h"
#include "ReflectionClasses.h"

#include <iostream>

int main() {
  // system("chcp 65001");           // 使用 65001 代码页（UTF-8）
  // Application app{1920, 1080};
  // app.Run();
  // return 0;
    D d;
    d.d6.emplace_back("d6");
    d.d6.emplace_back("d61");
    d.d6.emplace_back("d61");
    d.d6.emplace_back("d61");
    d.d6.emplace_back("d61");
    d.d1.push_back(new int32_t(1));
    d.d1.push_back(new int32_t(1));
    d.d4.emplace_back();
    d.d4.back().c1 = 99;
    d.d4.emplace_back();
    d.d4.back().c1 = 100;
    Json::Value value;
    JsonSerializer::Serialize(&d, value);
    std::cout << value;

    const D *d2 = JsonDeserializer::Deserialize<D>(value);
    std::cout << d2->d6[0];
}
