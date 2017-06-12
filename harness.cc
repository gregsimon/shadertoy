

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
// not needed #include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <functional>

const int WIDTH = 800;
const int HEIGHT = 600;

class HelloTriangleApplication {
public:
  void run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
  }

private:
  GLFWwindow* _window;

  void initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // don't use GL
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // disable window resize
    _window = glfwCreateWindow(WIDTH, HEIGHT, "Shadertoy", nullptr, nullptr);

  }

  void initVulkan() {

  }

  void mainLoop() {
    while (!glfwWindowShouldClose(_window)) {
      glfwPollEvents();
    }
  }

  void cleanup() {
    glfwDestroyWindow(_window);
    glfwTerminate();
  }
};

int main() {
  HelloTriangleApplication app;

  try {
    app.run();
  } catch (const std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
