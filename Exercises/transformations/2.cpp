#include <iostream>
#include <fstream>
#include <math.h>
#include "shaders/shader.h"
#include <glad/glad.h>
#include "stb_image/stb_image.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

// Update the glViewport based on the new size
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
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
        // vertex info     // color info    // Tex Coords
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
    };

    unsigned int indices[] = {
        0,1,2,
        1,2,3
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

    Shader shader = Shader("shaders/transformShader.vs", "shaders/mixValue.fs"); 
    shader.use();

    // Textures
    unsigned int texID[2];
    glGenTextures(2, texID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char*data = stbi_load("res/container.jpg", &width, &height, &nrChannels, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);    

    stbi_image_free(data);
    data = stbi_load("res/awesomeface.png", &width, &height, &nrChannels, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texID[1]);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // stbi_image_free(data);

    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    glBindVertexArray(VAO);

    // We use this to determine how the vertex information is processed. From learnopengl.com:
    // Position (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);   

    // Color (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);   

    // TexCoords (location = 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);   

    // Render loop
    float color = 0;
    float currentMixValue = 0.2f;
    shader.setFloat("mixValue", currentMixValue);
       
    while(!glfwWindowShouldClose(window)) {
        processInput(window);


        glGetUniformfv(shader.ID, glGetUniformLocation(shader.ID, "mixValue"), &currentMixValue);
        if (glfwGetKey(window, GLFW_KEY_UP)) {
            currentMixValue += 0.01f;
            glUniform1f(glGetUniformLocation(shader.ID, "mixValue"), currentMixValue);
        }
        else if (glfwGetKey(window, GLFW_KEY_DOWN)) {
            currentMixValue -= 0.01f;
            glUniform1f(glGetUniformLocation(shader.ID, "mixValue"), currentMixValue);
        }

        // create transformations
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first    
        transform = glm::scale(transform, glm::vec3(0.5f,0.5f,0.5f));    

        // get matrix's uniform location and set matrix
        //shader.use();    
        unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    

        glfwSwapBuffers(window);
        glfwPollEvents();    

        color += 0.01f; 

        glClearColor(sin(color/2) , sin(color / 3), sin(color / 4), 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Will now draw information present from ELEMENT ARRAY BUFFER
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first    
        transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
        transform = glm::rotate(transform, (float) sin(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    
    shader.cleanup();

    glfwTerminate();
 
    return 0;
}