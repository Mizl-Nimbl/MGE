#include "shader.hpp"

Shader::Shader(const char* vertpath, const char* fragpath)
{
    //shader
    std::cout << "Initializing Shader with vertex path: " << vertpath << " and fragment path: " << fragpath << std::endl;
    int  success;
    char infoLog[512];
    const char* vertexShaderSource = readShader(vertpath);
    const char* fragmentShaderSource = readShader(fragpath);
    if (!vertexShaderSource || !fragmentShaderSource) 
    {
        std::cerr << "Failed to read shader files." << std::endl;
    }
    std::cout << "shader files read" << std::endl;

    //vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); //this line is segfaulting
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex shader compilation failed:\n" << infoLog << std::endl;
    }
    //fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Fragment shader compilation failed:\n" << infoLog << std::endl;
    }
    //shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) 
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "Shader program linking failed:\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

const char* Shader::readShader(const char* shaderPath)
{
    std::ifstream shaderFile(shaderPath);
    if (!shaderFile.is_open()) {
        std::cerr << "Failed to open shader file: " << shaderPath << std::endl;
    }
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();
    std::string shaderCodeStr = shaderStream.str();
    char* shaderCode = new char[shaderCodeStr.size() + 1];
    std::copy(shaderCodeStr.begin(), shaderCodeStr.end(), shaderCode);
    shaderCode[shaderCodeStr.size()] = '\0';
    return shaderCode;
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}

void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setVec2(const std::string &name, glm::vec2 value) const
{ 
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value)); 
}

void Shader::setVec3(const std::string &name, glm::vec3 value) const
{ 
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value)); 
}

void Shader::setVec4(const std::string &name, glm::vec4 value) const
{ 
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value)); 
}

void Shader::setMat2(const std::string &name, glm::mat2 value) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat3(const std::string &name, glm::mat3 value) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}