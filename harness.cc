

#include <X11/Xutil.h>

#include <stdio.h>
#include <dlfcn.h>

#include <vulkan/vulkan.h>
#include <vulkan/vk_sdk_platform.h>

#define APP_SHORT_NAME "shadertoy"
#define APP_LONG_NAME "Vulkan Shadertoy Harness"


uint32_t enabled_layer_count = 0;
uint32_t enabled_extension_count = 0;
char extension_names[64] = {0};
char **instance_validation_layers = NULL;
VkInstance inst;

int main(int argc, char** argv) {

	VkResult err;
	
  VkApplicationInfo app;
  app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app.pNext = NULL;
  app.pApplicationName = APP_SHORT_NAME;
  app.applicationVersion = 0;
  app.pEngineName = APP_SHORT_NAME;
  app.engineVersion = 0;
  app.apiVersion = VK_API_VERSION_1_0;
  
  VkInstanceCreateInfo inst_info;
  inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  inst_info.pNext = NULL;
  inst_info.pApplicationInfo = &app;
  inst_info.enabledLayerCount = enabled_layer_count;
  inst_info.ppEnabledLayerNames = (const char *const *)instance_validation_layers;
  inst_info.enabledExtensionCount = enabled_extension_count;
  inst_info.ppEnabledExtensionNames = (const char *const *)extension_names;


	err = vkCreateInstance(&inst_info, NULL, &inst);

	return 0;
}