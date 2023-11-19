#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
private:
    unsigned int _program_id;
public:
    unsigned int get_program_id();
    void set_program_id(unsigned int value);

    Shader(std::string const &vertex_path, std::string const &fragment_path);
    void use();

    void set_uniform(std::string const &name, bool value);
    void set_uniform(std::string const &name, int value);
    void set_uniform(std::string const &name, float value);
};

Shader::Shader(std::string const &vertex_path, std::string const &fragment_path)
{
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream vertex_file;
    std::ifstream fragment_file;

    vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // Load shaders
        vertex_file.open(vertex_path);
        fragment_file.open(fragment_path);
        std::stringstream vertex_stream, fragment_stream;
        vertex_stream << vertex_file.rdbuf();
        fragment_stream << fragment_file.rdbuf();
        vertex_file.close();
        fragment_file.close();

        vertex_code = vertex_stream.str();
        fragment_code = fragment_stream.str();

        // Compile shaders
        unsigned int vertex_shader_id;
        vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
        char const *vertex_shader_source = vertex_code.c_str();
        glShaderSource(vertex_shader_id, 1, &vertex_shader_source, NULL);
        glCompileShader(vertex_shader_id);

        int success;
        char log[512];

        glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertex_shader_id, 512, NULL, log);
            std::cout << "ERROR::SHADER::VERTEX::COMPIATION FAILED" << std::endl << log << std::endl;
        }

        unsigned int fragment_shader_id;
        fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
        char const * fragment_shader_source = fragment_code.c_str();
        glShaderSource(fragment_shader_id, 1, &fragment_shader_source, NULL);
        glCompileShader(fragment_shader_id);
        glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragment_shader_id, 512, NULL, log);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPIATION FAILED" << std::endl << log << std::endl;
        }

        this->_program_id = glCreateProgram();
        glAttachShader(_program_id, vertex_shader_id);
        glAttachShader(_program_id, fragment_shader_id);
        glLinkProgram(_program_id);
        glGetProgramiv(_program_id, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(_program_id, 512, NULL, log);
            std::cout << "ERROR::PROGRAM::LINK FAILED" << std::endl << log << std::endl;
        }

        // Cleanup
        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
    }
    catch(std::ifstream::failure ex)
    {
        std::cerr << "ERROR::SHADER::FILEN_NOT_SUCCESSFULLY_READ" << ex.what() << std::endl;
    }
    
}

void Shader::use()
{
    glUseProgram(_program_id);
}

void Shader::set_uniform(std::string const &name, bool value)
{
    glUniform1i(glGetUniformLocation(_program_id, name.c_str()), value);
}

void Shader::set_uniform(std::string const &name, int value)
{
    glUniform1i(glGetUniformLocation(_program_id, name.c_str()), value);
}

void Shader::set_uniform(std::string const &name, float value)
{
    glUniform1f(glGetUniformLocation(_program_id, name.c_str()), value);
}

#endif