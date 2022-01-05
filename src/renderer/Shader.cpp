/**
 * @file Shader.cpp
 * @brief [fill in this secton]
 * Project: OpenGLTutorialCherno
 * Initial Version: 04/06/2021
 * @author Ryan Purse
 */

#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glew.h>


Shader::Shader(const std::string &filepath) : mFilePath(filepath), mRendererId(0)
{
    shaderProgramSource source = parseShader(filepath);
    mRendererId = createShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
    glDeleteProgram(mRendererId);
}

void Shader::bind() const
{
    glUseProgram(mRendererId);
}

void Shader::unBind()
{
    glUseProgram(0);
}

void Shader::setUniform(const std::string &name, int value)
{
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string &name, const glm::mat4 &mat4)
{
    // glm stores the matrix in the correct order so that we don't have to transpose it.
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat4[0][0]);
}

void Shader::setUniform(const std::string &name, const glm::vec4 &vec4)
{
    glUniform4f(getUniformLocation(name), vec4.x, vec4.y, vec4.z, vec4.w);
}

void Shader::setUniform(const std::string &name, float value)
{
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string &name, const glm::vec2 &value)
{
    glUniform2f(getUniformLocation(name), value.x, value.y);
}

void Shader::setUniform(const std::string &name, const glm::vec3 &value)
{
    glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
}

void Shader::setUniform(const std::string &name, const int *values, int count)
{
    glUniform1iv(getUniformLocation(name), count, values);
}

void Shader::setUniform(const std::string &name, const float *values, int count)
{
    glUniform1fv(getUniformLocation(name), count, values);
}

void Shader::setUniform(const std::string &name, const glm::vec3 &values, int count)
{
    glUniform3fv(getUniformLocation(name), count, &values[0]);
}

int Shader::getUniformLocation(const std::string &name)
{
    if (mUniformLocationCache.find(name) != mUniformLocationCache.end())
    {
        return mUniformLocationCache[name];
    }
    int location = glGetUniformLocation(mRendererId, name.c_str());
    if (location == -1)
    {
        debug::log("Warning: uniform '" + name + "' doesn't exist!", debug::severity::Minor);
    }
    mUniformLocationCache[name] = location;
    return location;
}

shaderProgramSource Shader::parseShader(const std::string &filepath)
{
    std::ifstream stream(filepath);

    if (stream.bad())
    {
        return {};
    }

    enum class shaderType { none = -1, vertex = 0, fragment = 1 };

    std::string line;
    std::stringstream ss[2];
    shaderType type = shaderType::none;
    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = shaderType::vertex;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = shaderType::fragment;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::compileShader(unsigned int type, const std::string &source)
{
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (GL_FALSE == result)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
//        char message[length];
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << "shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::createShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
