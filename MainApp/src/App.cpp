#include <glad/glad.h>

#include "camera/camera.h"
#include "shaders/shader.h"
#include "stb_image/stb_image.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <math.h>

using namespace std;

int screenWidth = 800, screenHeight = 600;
Shader shader;

// Update the glViewport based on the new size
void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void setMatrices(Camera cam) {
  unsigned int modelLoc, viewLoc, projLoc;

  modelLoc = glGetUniformLocation(shader.ID, "model");
  viewLoc = glGetUniformLocation(shader.ID, "view");
  projLoc = glGetUniformLocation(shader.ID, "projection");

  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cam.model));
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam.view));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(cam.projection));
}

int main() {
  // Window setup
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
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

  /* The first two parameters of glViewport set the location of the lower left
   corner of the window. The third and fourth parameter set the width and height
   of the rendering window in pixels, which we set equal to GLFW's window size.
   */
  glViewport(0, 0, screenWidth, screenHeight);

  // Used when window is resized
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

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

  // Associate ELEMENT ARRAY BUFFER TO EBO and pass in indices information
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
  // GL_STATIC_DRAW);

  shader = Shader("shaders/basicShader.vs", "shaders/mixValue.fs");
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
  unsigned char *data =
      stbi_load("res/container.jpg", &width, &height, &nrChannels, 0);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
  data = stbi_load("res/awesomeface.png", &width, &height, &nrChannels, 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texID[1]);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  // stbi_image_free(data);

  shader.setInt("texture1", 0);
  shader.setInt("texture2", 1);

  glBindVertexArray(VAO);

  // We use this to determine how the vertex information is processed. From
  // learnopengl.com: Position (location = 0)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // TexCoords (location = 2)
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glEnable(GL_DEPTH_TEST);

  // Render loop
  float color = 0;
  float currentMixValue = 0.2f;
  shader.setFloat("mixValue", currentMixValue);

  // Create canera
  Camera cam = Camera();

  // Used for getting mouse input
  glfwSetCursorPosCallback(window, cam.mouse_callback);

  // Set view
  cam.setView(glm::lookAt(cam.pos, cam.target, glm::vec3(0.0f, 1.0f, 0.0f)));

  // create for the model transformations
  glm::mat4 transform = glm::mat4(
      1.0f); // make sure to initialize matrix to identity matrix first

  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  // get matrix's uniform location and set matrix
  // shader.use();
  unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

  float previousTime = glfwGetTime();
  float deltaTime = 0;

  while (!glfwWindowShouldClose(window)) {
    deltaTime = glfwGetTime() - previousTime;
    previousTime = glfwGetTime();

    processInput(window);
    cam.updateCamera(window, deltaTime);

    glGetUniformfv(shader.ID, glGetUniformLocation(shader.ID, "mixValue"),
                   &currentMixValue);
    if (glfwGetKey(window, GLFW_KEY_UP)) {
      currentMixValue += 0.01f;
      glUniform1f(glGetUniformLocation(shader.ID, "mixValue"), currentMixValue);
    } else if (glfwGetKey(window, GLFW_KEY_DOWN)) {
      currentMixValue -= 0.01f;
      glUniform1f(glGetUniformLocation(shader.ID, "mixValue"), currentMixValue);
    }

    // Handle transforms
    /*
    cam.setView(glm::lookAt(cam.pos, cam.pos + cam.front, cam.up));

    cam.setProjection(glm::perspective(glm::radians(45.0f),
                                       (float)screenWidth / (float)screenHeight,
                                       0.01f, 100.0f));
                                       */

    glfwSwapBuffers(window);
    glfwPollEvents();

    color += 0.01f;

    glClearColor(sin(color / 2), sin(color / 3), sin(color / 4), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Will now draw information present from ELEMENT ARRAY BUFFER
    for (unsigned int i = 0; i < 10; i++) {
      glm::mat4 model;
      float angle = 20.0f * i + (float)glfwGetTime() * 50.0f;
      model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);

      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

      cam.setModel(model);

      setMatrices(cam);

      shader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
  }

  shader.cleanup();

  glfwTerminate();

  return 0;
}
