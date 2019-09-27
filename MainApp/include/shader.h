#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader 
{
    public:
    // the program ID
    unsigned int ID;

    // the ID of the shaders
    GLuint vertexShader;
    GLuint fragmentShader;
    
    // constructor reads and builds the shader
    Shader(string vertexPath, string fragmentPath);
    
    // use/activate the shader
    void use();
    
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

    // Delete shader
    void cleanup();
};

#endif