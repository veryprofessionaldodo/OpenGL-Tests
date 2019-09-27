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
    void setBool(const string &name, bool v) const;
    void setInt(const string &name, int v) const;
    void setFloat(const string &name, float v) const;
    void setUniform3f(const string &name, float v1, float v2, float v3) const;

    // Delete shader
    void cleanup();
};

#endif