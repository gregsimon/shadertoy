#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

struct QueueFamilyIndices {
    int graphicsFamily = -1;
    int presentFamily = -1;

    bool isComplete() {
        return graphicsFamily >= 0 && presentFamily >= 0;
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};


class ShaderToyVulkanHarness {
public:

  ShaderToyVulkanHarness(bool bValidating = false);
  virtual ~ShaderToyVulkanHarness();

  virtual VkSurfaceKHR createSurface(VkInstance) = 0;
  virtual std::vector<const char*> getRequiredExtensions() = 0;

  void init(int width, int height, const std::string& shaderSPVfile = std::string());
  void drawFrame();
  void updateUniformBuffer();
  void resize(int width, int height);
  bool isValidating() const { return _isValidating; }

private:  
  void createInstance();
  void setupDebugCallback();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createSwapChain();
  void createImageViews();
  void createRenderPass();
  void createDescriptorSetLayout();
  void createGraphicsPipeline();
  void createFramebuffers();
  void createCommandPool();
  void createVertexBuffer();
  void createIndexBuffer();
  void createUniformBuffer();
  void createDescriptorPool();
  void createDescriptorSet();
  void createCommandBuffers();
  void createSemaphores();
  void cleanup();
  void cleanupSwapChain();

  void recreateSwapChain();


  VkShaderModule createShaderModule(const std::vector<char>& code);
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
  void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
  bool isDeviceSuitable(VkPhysicalDevice device);
  bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);  
  bool checkValidationLayerSupport();
  uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
  void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);

 
private:
  int _width;
  int _height;
  std::string _shaderSPVFn;

  bool _isValidating = false;

  VkInstance _instance;
  VkDebugReportCallbackEXT _debugCallback;
  VkSurfaceKHR _surface;

  VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
  VkDevice device;

  VkQueue _graphicsQueue;
  VkQueue _presentQueue;
  
  VkSwapchainKHR _swapChain;
  std::vector<VkImage> _swapChainImages;
  VkFormat _swapChainImageFormat;
  VkExtent2D _swapChainExtent;
  std::vector<VkImageView> _swapChainImageViews;
  std::vector<VkFramebuffer> _swapChainFramebuffers;
  
  VkRenderPass _renderPass;
  VkDescriptorSetLayout _descriptorSetLayout;
  VkPipelineLayout _pipelineLayout;
  VkPipeline _graphicsPipeline;
  
  VkCommandPool _commandPool;
  std::vector<VkCommandBuffer> _commandBuffers;

  VkSemaphore _imageAvailableSemaphore;
  VkSemaphore _renderFinishedSemaphore;

  VkBuffer _vertexBuffer;
  VkDeviceMemory _vertexBufferMemory;
  VkBuffer _indexBuffer;
  VkDeviceMemory _indexBufferMemory;

  VkBuffer _uniformBuffer;
  VkDeviceMemory _uniformBufferMemory;

  VkDescriptorPool _descriptorPool;
  VkDescriptorSet _descriptorSet;
};

