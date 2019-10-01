#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include "glm/glm.hpp"
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class Camera {
public:
  Camera();
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;

  // Mouse Input
  bool firstMouse;
  float lastX;
  float lastY;
  float yaw, pitch;

  float speed;
  glm::vec3 front;
  glm::vec3 up;

  glm::vec3 pos;
  glm::vec3 target;

  void setModel(glm::mat4 newModel);
  void setView(glm::mat4 newView);
  void setProjection(glm::mat4 newProjection);

  void updateCamera(GLFWwindow *window, float deltaTime);
  void mouse_callback(GLFWwindow *window, double xpos, double ypos);
};

#endif