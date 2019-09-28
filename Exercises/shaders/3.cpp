#include <iostream>
#include <fstream>
#include <math.h>
#include "shaders/shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

// Update the glViewport based on the new size
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


int main() {
    // Window setup
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set the current context to the windo's
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }   

    /* The first two parameters of glViewport set the location of the lower left corner of the window.
     The third and fourth parameter set the width and height of the rendering window in pixels,
      which we set equal to GLFW's window size. 
     */
    glViewport(0, 0, 800, 600);

    // Used when window is resized
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);  

    float vertices[] = {
        // vertex info   
        -0.5f, -0.5f, 0.0f,
        -0.0f, 0.5f, 0.0f, 
        0.5f, -0.5f, 0.0f
    };

    unsigned int indices[] = {
        0,1,2
    };
    
    // Generate vertex buffer object 
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);  
    
    glBindVertexArray(VAO);

    // Assign buffer tyoe to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copy information from vertices to the vbo
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Generate EBO for index storage
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    
    // Associate ELEMENT ARRAY BUFFER TO EBO and pass in indices information
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    Shader shader = Shader("shaders/vertexColPosition.vs", "shaders/basicShader.fs"); 
    shader.use();

    glBindVertexArray(VAO);

    // We use this to determine how the vertex information is processed. From learnopengl.com:
    // Position (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);   

    // Render loop
    float color = 0;

    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glfwSwapBuffers(window);
        glfwPollEvents();    

        color += 0.01f; 

        glClearColor(sin(color/2) , sin(color / 3), sin(color / 4), 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float time = glfwGetTime();
        /* 
        float red = sin(time);
        float green = sin(time/2);
        float blue = sin(time*2);

        unsigned int shader_color_location = glGetUniformLocation(shader_program, "shaderColor");

        glUniform4f(shader_color_location, red, green, blue, 1.0f);
        */

        shader.setUniform3f("offset",sin(time)*0.5f, sin(time/1.3f)*0.5f, sin(time*1.3f)*0.5f);

        // Will now draw information present from ELEMENT ARRAY BUFFER
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    
    shader.cleanup();

    glfwTerminate();
 
    return 0;
}