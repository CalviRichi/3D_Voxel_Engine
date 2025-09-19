#include "shader.h"
#include "commonInclude.h"
#include "vendor/glm/gtc/type_ptr.hpp"
#include <iostream>
#include <fstream>
#include <sstream>



Shader::Shader(const std::string& filepath) 
    : m_Filepath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = parseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
    if (m_RendererID == 0) {
        std::cerr << "Shader program is invalid. Exiting constructor." << std::endl;
        return;
    }
}

Shader::~Shader() {
    GLCALL(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const {
   // std::cout << m_RendererID << std::endl;
    GLCALL(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
    GLCALL(glUseProgram(0));
}

int Shader::getUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        //std::cout << "name: " << m_UniformLocationCache[name] << std::endl;
        return m_UniformLocationCache[name];
    }

    GLCALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: uniform " << name << " does not exist" << std::endl;
    }

    m_UniformLocationCache[name] = location;
    return location;
}

void Shader::setUniform1i(const std::string &name, int value)
{
    GLCALL(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform1f(const std::string &name, float value)
{
    GLCALL(glUniform1f(getUniformLocation(name), value));
}

void Shader::setUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
    GLCALL(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const std::string &name, const glm::mat4 &mat)
{
    GLCALL(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat)));
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    GLuint program = glCreateProgram();
        GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
        GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

        if (vs == 0 || fs == 0) {
            std::cout << "Shader creation failed. Aborting program creation.\n";
            if (vs) glDeleteShader(vs);
            if (fs) glDeleteShader(fs);
            glDeleteProgram(program);
            return 0;
        }

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);

        // Check linking
        GLint linkStatus;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus == GL_FALSE) {
            GLint length;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

            std::vector<char> message(length);
            glGetProgramInfoLog(program, length, &length, message.data());

            std::cout << "Shader program linking failed:\n";
            std::cout << message.data() << "\n";

            glDeleteProgram(program);
            glDeleteShader(vs);
            glDeleteShader(fs);
            return 0;
        }

        glValidateProgram(program);
        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
}

unsigned int Shader::compileShader(unsigned int type,const std::string& source) {
    GLuint id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        // Check compilation
        GLint result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            GLint length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

            std::vector<char> message(length);
            glGetShaderInfoLog(id, length, &length, message.data());

            std::cout << "--------------------\n";
            std::cout << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
                      << " shader compilation failed:\n";
            std::cout << "Source:\n" << source << "\n";
            std::cout << "Log:\n" << message.data() << "\n";
            std::cout << "--------------------\n";

            glDeleteShader(id);
            return 0;
        }

        return id;
}

ShaderProgramSource Shader::parseShader(const std::string& filepath) {
    // this is a C++ way to do this, I could search
    // up the C way to do it
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    
    //std::ifstream stream(filepath);
    if (!stream.is_open()) {
        std::cerr << "Failed to open shader file: " << filepath << std::endl;
    }

    while(getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
            // set mode to vertex
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
                ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
    // no need to make a full struct

}
