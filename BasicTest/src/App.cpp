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

void render_loop(GLFWwindow* window) {
    int color = 0;

    while(!glfwWindowShouldClose(window)) {
        process_input(window);

        glfwSwapBuffers(window);
        glfwPollEvents();    

        color += 1; 

        glClearColor(((color / 2) % 255)/255.0f, ((color / 3) % 255)/255.0f, ((color / 4) % 255)/255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}

string get_shader_source(string filename) {
    string filepath = "/home/workingdodo/OpenGL-Tests/BasicTest/src/shaders/" + filename; 
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

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
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

    // Create shader program
    unsigned int shader_program;
    shader_program = glCreateProgram();

    // Attach the new shaders
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    
    // Link the final program
    glLinkProgram(shader_program);

    // Now we use the program (after we set all the states including the shaders)
    glUseProgram(shader_program);

    glBindVertexArray(VAO);

    // Now that everything is linked, delete the shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);  

    // We use this to determine how the vertex information is processed. From learnopengl.com:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);   

    render_loop(window);

    glfwTerminate();
 
    return 0;
}