#include "ShaderProgram.h"

void ShaderProgram::loadShaderFromFile(std::string filePath, GLuint shaderType)
{
    std::string line;
    std::string shaderCode;
    std::ifstream inF(filePath); // окрываем файл для чтения
    if (inF.is_open())
    {

        while (getline(inF, line))
        {
            shaderCode += line + "\n";
        }
    }
    inF.close();


    const  char* shaderSource = shaderCode.c_str();
    int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
    };


    this->shaders.push_back(shader);

}

void ShaderProgram::setObjectMPV(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    glUniformMatrix4fv(getUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(getUniform("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(getUniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void ShaderProgram::setUniformV1(std::string name, float value)
{
}
void ShaderProgram::setUniformMat4(std::string name, glm::mat4 value)
{
    glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, glm::value_ptr(value));
}
void ShaderProgram::setUniformV3(std::string name, glm::vec3 value)
{
    glUniform3f(getUniform(name), value.x, value.y, value.z);
}

//void ShaderProgram::setSampler(std::string name, unsigned int samplerIndex, unsigned int sampler)
//{
//}



void ShaderProgram::setSampler(std::string name, unsigned int samplerIndex,   Texture& sampler)
{
    glUniform1i(getUniform(name), samplerIndex);
 
    sampler.bind(samplerIndex);
}

void ShaderProgram::attachAndCompile()
{
  this->shaderProgram  = glCreateProgram();
  for (auto i = shaders.begin(); i != shaders.end(); i++)
  {

      glAttachShader(this->shaderProgram, *i);
  }

    glLinkProgram(this->shaderProgram);

}

void ShaderProgram::use()
{
    glUseProgram(this->shaderProgram);
}

unsigned int ShaderProgram::getUniform(std::string name)
{
    return  glGetUniformLocation(this->shaderProgram, name.c_str());
}
