#include "camera.h"

Camera::Camera() {
  model = glm::mat4(1.0f);
  view = glm::mat4(1.0f);
  projection = glm::mat4(1.0f);

  pos = glm::vec3(0.0f, 1.0f, 3.0f);
  target = glm::vec3(0.0f, 0.0f, 0.0f);

  front = glm::vec3(0.0f, 0.0f, -1.0f);
  up = glm::vec3(0.0f, 1.0f, 0.0f);

  speed = 10.0f;

  yaw = 0;
  pitch = 0;
}

void Camera::setModel(glm::mat4 newModel) { model = newModel; }
void Camera::setView(glm::mat4 newView) { view = newView; }
void Camera::setProjection(glm::mat4 newProjection) {
  projection = newProjection;
}

void Camera::updateCamera(GLFWwindow *window, float deltaTime) {
  cout << "yee yaw " << yaw << " pitch perfect " << pitch << endl;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    pos += speed * deltaTime * front;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    pos -= speed * deltaTime * front;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    pos -= glm::normalize(glm::cross(front, up)) * deltaTime * speed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    pos += glm::normalize(glm::cross(front, up)) * deltaTime * speed;
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    pos -= speed * deltaTime * up;
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    pos += speed * deltaTime * up;

  cout << pos.x << "," << pos.y << "," << pos.z << endl;

  // Handle transforms
  setView(glm::lookAt(pos, pos + front, up));

  setProjection(glm::perspective(glm::radians(45.0f), (float)800 / (float)600,
                                 0.01f, 100.0f));
}
