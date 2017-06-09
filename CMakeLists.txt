# hello

cmake_minimum_required(VERSION 3.7)

include_directories($ENV{VULKAN_SDK}/include)
LINK_DIRECTORIES($ENV{VULKAN_SDK}/lib)



SET(CMAKE_CXX_FLAGS "-std=c++11") 


add_executable(shaderharness harness.cc)

target_link_libraries(shaderharness libvulkan.so)