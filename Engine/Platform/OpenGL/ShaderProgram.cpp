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

void ShaderProgram::Use() const {
  glUseProgram(m_id);
}

void ShaderProgram::SetMatrix4(const std::string &name, const Matrix4 &matrix) const {
  const auto location = glGetUniformLocation(m_id, name.c_str());
  glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void ShaderProgram::SetInt(const std::string &name, const int32_t value) const {
  const auto location = glGetUniformLocation(m_id, name.c_str());
  glUniform1i(location, value);
}

} // namespace Platform::GL
