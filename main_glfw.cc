// Vulkan Harness to run Vulkan-compatible Shadertoy shaders

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "shadertoy.h"

#include <stdexcept>
#include <iostream>
#include <chrono>

const int WIDTH = 800;
const int HEIGHT = 600;


class ShaderToyVulkanHarnessGLFW : public ShaderToyVulkanHarness {
public:
  ShaderToyVulkanHarnessGLFW(GLFWwindow* win, bool validating) 
    : ShaderToyVulkanHarness(validating),_window(win) {}
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


int main(int argc, char** argv) {
  ShaderToyVulkanHarness * shadertoy;
  int width = WIDTH;
  int height = HEIGHT;
  std::string spv_filename;

  // parse args.
  for (int i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch( argv[i][1]) {
        case 's':
          width = atoi(argv[i+1]);
          height = atoi(argv[i+2]);
          i++;
          break;
        case 'h':
          std::cout << "Usage: " << argv[0] 
            << "[-s <width> <height]"
            << " [spv filename]"
            << std::endl;
          return 0;
          break;
        default:
          std::cerr << "Unknown argument: " << argv[i] << std::endl;
      }
      i++; // skip over the (default) single value
    } else {
      spv_filename = argv[i];
    }
  }

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  GLFWwindow* window = glfwCreateWindow(width, height, "Shadertoy Vulkan", nullptr, nullptr);

  try {
    shadertoy = new ShaderToyVulkanHarnessGLFW(window,/* validating = */ true);
 
    glfwSetWindowUserPointer(window, shadertoy);
    glfwSetWindowSizeCallback(window, onWindowResized);

    shadertoy->init(width, height, spv_filename);

    // Main loop
    auto last_time = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();

      shadertoy->updateUniformBuffer();
      shadertoy->drawFrame();
      auto this_time = std::chrono::high_resolution_clock::now();

      float delta = std::chrono::duration_cast<std::chrono::milliseconds>(this_time - last_time).count() / 1000.0f;
      float fps = 1.0f / delta;

      std::cout << fps << " FPS" << std::endl;

      last_time = this_time;
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
