
#include "shadertoy.h"

#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;



class ShaderToyVulkanHarnessFuchsia : public ShaderToyVulkanHarness {
public:
  ShaderToyVulkanHarnessFuchsia(bool validating)
    : ShaderToyVulkanHarness(validating) {}
  ~ShaderToyVulkanHarnessFuchsia() {}

  virtual VkSurfaceKHR createSurface(VkInstance inst) {
    VkSurfaceKHR surface = 0;
    VkMagmaSurfaceCreateInfoKHR createInfo = {
      .sType = VK_STRUCTURE_TYPE_MAGMA_SURFACE_CREATE_INFO_KHR,
      .pNext = nullptr,
    };
    VkResult err = vkCreateMagmaSurfaceKHR(inst, &createInfo, nullptr, &surface);
    if (err)
      throw std::runtime_error("Magma surface creation failed");
    return surface;
  }

  virtual std::vector<const char*> getRequiredExtensions() {
    std::vector<const char*> extensions;

    std::cout << "getRequiredExtensions" << std::endl;

    VkResult err;
    uint32_t instance_extension_count = 0;
    err = vkEnumerateInstanceExtensionProperties(NULL, &instance_extension_count, NULL);

    if (instance_extension_count > 0) {
      VkExtensionProperties *instance_extensions = reinterpret_cast<VkExtensionProperties*>(
          malloc(sizeof(VkExtensionProperties) * instance_extension_count));
      err = vkEnumerateInstanceExtensionProperties(NULL, &instance_extension_count, instance_extensions);
      assert(!err);
      for (uint32_t i = 0; i < instance_extension_count; i++) {
          if (!strcmp(VK_KHR_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
            extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
          }
          if (!strcmp(VK_KHR_MAGMA_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
            extensions.push_back(VK_KHR_MAGMA_SURFACE_EXTENSION_NAME);
          }
          if (!strcmp(VK_EXT_DEBUG_REPORT_EXTENSION_NAME, instance_extensions[i].extensionName)) {
            if (isValidating()) {
              extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
          }
        }
      } // for(instance_extension_count)

      free(instance_extensions);
    }

    /*unsigned int glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (unsigned int i = 0; i < glfwExtensionCount; i++) {
        extensions.push_back(glfwExtensions[i]);
    }

    if (isValidating()) {
        extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }*/

    return extensions;
  }
private:
};


int main(int argc, char** argv) {
  
  try {
    std::cout << "Hello from shadertoy harness!" << std::endl;

    std::unique_ptr<ShaderToyVulkanHarnessFuchsia>
            shadertoy(new ShaderToyVulkanHarnessFuchsia(true));

    shadertoy->init(WIDTH, HEIGHT);


  } catch (const std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return 0;
}

