#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

// Update the glViewport based on the new size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

string get_shader_source(string filename) {
    string filepath = "shaders/" + filename; 
    ifstream file(filepath);
    string shader_source = "";
    string line;

    if (file.is_open())  {
        while (getline (file,line) ) {
            shader_source += line + '\n';
            // cout << line << endl;
        }
        file.close();
    }
    return shader_source;
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

       // Compile vertex shader in run time
    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    
    string vertex_shader_content = get_shader_source("vertex_shader.glsl");
    const char* vertex_shader_source = vertex_shader_content.c_str();
    
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    // Compile Fragment shader in run time
    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    string fragment_shader_content = get_shader_source("fragment_shader.glsl");
    const char* fragment_shader_source = fragment_shader_content.c_str();

    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    unsigned int fragment_yellow;
    fragment_yellow = glCreateShader(GL_FRAGMENT_SHADER);

    string fragment_yellow_content = get_shader_source("fragment_yellow.glsl");
    const char* fragment_yellow_source = fragment_yellow_content.c_str();

    glShaderSource(fragment_yellow, 1, &fragment_yellow_source, NULL);
    glCompileShader(fragment_yellow);
    
    // Check to see if shader was compiled
    int  success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Check to see if shader was compiled
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Check to see if shader was compiled
    glGetShaderiv(fragment_yellow, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create shader program
    unsigned int shader_program;
    shader_program = glCreateProgram();

    // Attach the new shaders
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_yellow);
    // Link the final program
    glLinkProgram(shader_program);
    glUseProgram(shader_program);
        
    float vertices1[] = {
        -1.0f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.0f, -0.5f, 0.0f
    };

    float vertices2[] = {
        0.0f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        1.0f, -0.5f, 0.0f
    };

    // Generate vertex buffer object 
    unsigned int VBOs[2];
    glGenBuffers(2, VBOs);

    unsigned int VAOs[2];
    glGenVertexArrays(2, VAOs);  

    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);   

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);   

    // Render loop
    int color = 0;

    while(!glfwWindowShouldClose(window)) {
        process_input(window);

        glfwSwapBuffers(window);
        glfwPollEvents();    

        color += 1; 

        glClearColor(((color / 2) % 255)/255.0f, ((color / 3) % 255)/255.0f, ((color / 4) % 255)/255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDetachShader(shader_program, fragment_yellow);
        glAttachShader(shader_program, fragment_shader);
        glLinkProgram(shader_program);
        
        glBindVertexArray(VAOs[0]);
        
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices1));

        glDetachShader(shader_program, fragment_shader);
        glAttachShader(shader_program, fragment_yellow);
        glLinkProgram(shader_program);
        
        glBindVertexArray(VAOs[1]);
        
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices2));
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    //glDeleteShader(fragment_yellow);

    glfwTerminate();
 
    return 0;
}