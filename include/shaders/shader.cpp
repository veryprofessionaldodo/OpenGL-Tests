#include "shader.h"

using namespace std; 

Shader::Shader() 
{

}

Shader::Shader(string vertexPath, string fragmentPath) 
{
    // Load from file
    ifstream vertexFile(vertexPath);
    ifstream fragmentFile(fragmentPath);

    // ensure ifstream objects can throw exceptions:
    //vertexFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    //fragmentFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    
    string vertexShaderCode = "";
    string fragmentShaderCode = "";
    string line;
    try {
        if (vertexFile.is_open())  {
            while (getline (vertexFile,line) ) {
                vertexShaderCode += line + '\n';
            }
            vertexFile.close();
        }
    } catch (exception e) {
        cout << "Error reading from vertex shader file.\n";
    }
    try {
        if (fragmentFile.is_open())  {
            while (getline (fragmentFile,line) ) {
                fragmentShaderCode += line + '\n';
            }
            fragmentFile.close();
        }
    } catch (exception e) {
        cout << "Error reading from fragment shader file.\n";
    }

    const char* vertexShaderSource = vertexShaderCode.c_str();
    const char* fragmentShaderSource = fragmentShaderCode.c_str();

    // Compile Shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    vertexShader = glCreateShader(GL_VERTEX_SHADER); 
   
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check to see if shader was compiled
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Check to see if shader was compiled
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }  

    ID = glCreateProgram();
    
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);

    glLinkProgram(ID);

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

void Shader::setUniform3f(const string &name, float v1, float v2, float v3) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3); 
}

void Shader::setMat4(const string &name, glm::mat4 mat4) const
{   
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat4));
}

void Shader::cleanup() 
{
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}