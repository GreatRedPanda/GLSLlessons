#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
{
    program = glCreateProgram();
}

void ShaderProgram::addShader(std::string path, int shaderType)
{
    std::string shaderCode;
    std::string line;
    std::ifstream in(path); // окрываем файл для чтения
    if (in.is_open())
    {

        while (getline(in, line))
        {
            shaderCode += line + "\n";
        }
    }
    in.close();
    const  char* shaderCodeChar = shaderCode.c_str();
    int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCodeChar, NULL);
    glCompileShader(shader);
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
    glAttachShader( this->program,  shader);
    glLinkProgram(this->program);
}

void ShaderProgram::use()
{

    glUseProgram(program);
}

int ShaderProgram::getUniform(std::string name)
{
return    glGetUniformLocation(this->program, name.c_str());
}
