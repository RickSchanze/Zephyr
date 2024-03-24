#pragma once
#include "CommonMacro.h"

#include <string>
#include <vector>

class A {
public:
    PROPERTY()
    int32_t a1 = 1;
    PROPERTY()
    int8_t a2 = 2;
    PROPERTY()
    int16_t a3 = 3;
    PROPERTY()
    float a4 = 4;
    PROPERTY()
    double a5 = 5;
};

class B : public A {
public:
    PROPERTY()
    int32_t b1 = 6;
    PROPERTY()
    int8_t b2 = 7;
    PROPERTY()
    int16_t b3 = 8;
    PROPERTY()
    float b4 = 9;
    PROPERTY()
    double b5 = 10;
};

class C : public B {
public:
    PROPERTY()
    int32_t c1 = 11;
    PROPERTY()
    int8_t c2 = 12;
    PROPERTY()
    int16_t c3 = 13;
    PROPERTY()
    float c4 = 14;
    PROPERTY()
    B* c5 = nullptr;
};

class D : public A {
public:
    PROPERTY() std::vector<int32_t*> d1;
    PROPERTY() std::vector<int32_t> d2 = {1,2,3,4,5};
    PROPERTY() std::string d3;
    PROPERTY() C d4{};
    PROPERTY() std::vector<std::string*> d5;
};
