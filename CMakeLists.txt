# shaderspy harness

cmake_minimum_required(VERSION 3.7)

find_package(PkgConfig REQUIRED)
find_package(X11 REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)




include_directories($ENV{VULKAN_SDK}/include)
LINK_DIRECTORIES($ENV{VULKAN_SDK}/lib)



SET(CMAKE_CXX_FLAGS "-std=c++11") 


add_executable(shaderharness harness.cc)

target_link_libraries(shaderharness ${GLFW_LIBRARIES} ${X11_LIBRARIES} 
  libXrandr.so libXi.so libXinerama.so libXcursor.so libvulkan.so libdl.so libpthread.so)
