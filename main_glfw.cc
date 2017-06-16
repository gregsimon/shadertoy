// Vulkan Harness to run Vulkan-compatible Shadertoy shaders

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "shadertoy.h"

#include <stdexcept>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;


class ShaderToyVulkanHarnessGLFW : public ShaderToyVulkanHarness {
public:
  ShaderToyVulkanHarnessGLFW(GLFWwindow* win, int width, int height) 
    : ShaderToyVulkanHarness(width, height),_window(win) {}
  ~ShaderToyVulkanHarnessGLFW() {}

  virtual VkSurfaceKHR createSurface(VkInstance inst) {
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(inst, _window, nullptr, &surface) != VK_SUCCESS) {
      throw std::runtime_error("failed to create window surface!");
    }
    return surface;
  }

  virtual std::vector<const char*> getRequiredExtensions() {
    std::vector<const char*> extensions;

    unsigned int glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (unsigned int i = 0; i < glfwExtensionCount; i++) {
        extensions.push_back(glfwExtensions[i]);
    }

    if (isValidating()) {
        extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }

    return extensions;
  }
private:
  GLFWwindow* _window;
};


static void onWindowResized(GLFWwindow* window, int width, int height) {
  if (width == 0 || height == 0) return;
  
  ShaderToyVulkanHarness* shadertoy = reinterpret_cast<ShaderToyVulkanHarness*>(glfwGetWindowUserPointer(window));
  if (shadertoy)
    shadertoy->resize(width, height);
}


int main() {
  ShaderToyVulkanHarness * shadertoy;

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Shadertoy Vulkan", nullptr, nullptr);

  try {
    shadertoy = new ShaderToyVulkanHarnessGLFW(window, WIDTH, HEIGHT);
 
    glfwSetWindowUserPointer(window, shadertoy);
    glfwSetWindowSizeCallback(window, onWindowResized);

    shadertoy->initVulkan();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();

      shadertoy->updateUniformBuffer();
      shadertoy->drawFrame();
    }


    delete shadertoy;

  } catch (const std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}
