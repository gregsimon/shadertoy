# shaderspy harness
# apt-get install libglm-dev
# https://github.com/glfw/glfw -> build

cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig REQUIRED)
find_package(X11 REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)


add_custom_command(OUTPUT shaders/vert.spv 
    COMMAND glslangValidator -V shaders/shader.vert -o shaders/vert.spv
    DEPENDS shaders/shader.vert)
set_property(SOURCE harness.cc APPEND PROPERTY OBJECT_DEPENDS shaders/vert.spv)

#add_custom_command(OUTPUT frag.spv 
#    COMMAND glslangValidator -V shaders/shader.frag
#    DEPENDS shaders/shader.frag)
#set_property(SOURCE harness.cc APPEND PROPERTY OBJECT_DEPENDS frag.spv)


add_custom_command(OUTPUT shaders/seascape.spv 
    COMMAND glslangValidator -V shaders/seascape.frag -o shaders/seascape.spv
    DEPENDS shaders/seascape.frag)
set_property(SOURCE harness.cc APPEND PROPERTY OBJECT_DEPENDS shaders/seascape.spv)



include_directories($ENV{VULKAN_SDK}/include)
LINK_DIRECTORIES($ENV{VULKAN_SDK}/lib)

SET(CMAKE_CXX_FLAGS "-std=c++11") 

add_executable(harness harness.cc)

target_link_libraries(harness ${GLFW_LIBRARIES} ${X11_LIBRARIES} 
  libXrandr.so libXi.so libXinerama.so libXcursor.so libvulkan.so libdl.so libpthread.so)
