/**
 * @file ShaderProgram.cpp
 * @author Echo
 * @Date 2024/3/6
 * @brief
 */

#include "ShaderProgram.h"

namespace Platform::GL
{

ShaderProgram::ShaderProgram()
{
    m_id = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_id);
}

void ShaderProgram::Link() const
{
    glLinkProgram(m_id);
}

void ShaderProgram::Use() const
{
    glUseProgram(m_id);
}

}
