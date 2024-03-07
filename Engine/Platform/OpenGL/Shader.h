/**
 * @file Shader.h
 * @author Echo
 * @Date 2024/3/4
 * @brief
 */

#ifndef ZEPHYR_SHADER_H
#define ZEPHYR_SHADER_H

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>

#ifndef __glad_h_
#include "glad/glad.h"
#endif

#include "Logger/Logger.h"

namespace Platform::GL
{

enum class EShaderType
{
    Vertex,
    Fragment
};

template <EShaderType shaderType>
class Shader
{
public:
    /**
     * 构造shader
     * @param shader_path shader文件路径
     */
    explicit Shader(const std::wstring_view shader_path)
    {
        m_shader_type = shaderType;
        m_shader_path = shader_path;
        switch (m_shader_type)
        {
        case EShaderType::Vertex:
            m_id = glCreateShader(GL_VERTEX_SHADER);
            break;
        case EShaderType::Fragment:
            m_id = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        }
    }

    ~Shader()
    {
        glDeleteShader(m_id);
    }

    /** 使用当前Shader */
    void Use()
    {
        if (IsValid())
        {
            glUseProgram(m_id);
        }
        else
        {
            ZEPHYR_LOG_ERROR("{} Shader {} is not valid, can't use", GetShaderTypeString(), m_shader_path);
        }
    }

    /** 初始化Shader 包括读取shader文件等 */
    void Initialize()
    {
        // 判断文件是否合规
        namespace fs = std::filesystem;
        if (!fs::exists(m_shader_path))
        {
            ZEPHYR_LOG_ERROR("{} Shader file not exists: {}", GetShaderTypeString(), m_shader_path);
            return;
        }
        if (fs::is_directory(m_shader_path))
        {
            ZEPHYR_LOG_ERROR("{} Shader file is a directory: {}", GetShaderTypeString(), m_shader_path);
            return;
        }

        // 初始化shader
        std::string shader_code;
        std::ifstream shader_file;
        shader_file.open(m_shader_path);
        std::stringstream shader_stream;
        shader_stream << shader_file.rdbuf();
        shader_file.close();
        shader_code = shader_stream.str();

        const char *shader_code_c = shader_code.c_str();
        int32_t success;
        glShaderSource(m_id, 1, &shader_code_c, nullptr);
        glCompileShader(m_id);
        glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char info_log[512];
            glGetShaderInfoLog(m_id, 512, nullptr, info_log);
            ZEPHYR_LOG_ERROR("{} Shader compile failed: {}", GetShaderTypeString(), info_log);
            return;
        }
        m_initialized = true;
    }

    EShaderType GetShaderType()const
    {
        return m_shader_type;
    }

    std::wstring GetShaderPath()
    {
        return m_shader_path;
    }

    bool IsValid()const
    {
        return m_initialized;
    }

    std::string GetShaderTypeString()const
    {
        switch (m_shader_type)
        {
        case EShaderType::Vertex:
            return "Vertex";
        case EShaderType::Fragment:
            return "Fragment";
        }
        return "";
    };

    uint32_t GetId() const
    {
        return m_id;
    }

protected:
    uint32_t m_id{0};
    EShaderType m_shader_type;
    std::wstring m_shader_path;
    bool m_initialized = false;
};

typedef Shader<EShaderType::Vertex> VertexShader;
typedef Shader<EShaderType::Fragment> FragmentShader;

}

#endif // ZEPHYR_SHADER_H
