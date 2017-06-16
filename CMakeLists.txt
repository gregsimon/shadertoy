# shaderspy harness
# apt-get install libglm-dev
# https://github.com/glfw/glfw -> build

cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig REQUIRED)
find_package(X11 REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)


function (add_shader in_name out_name)
  add_custom_command(OUTPUT ${out_name} COMMAND glslangValidator -V ${in_name} -o ${out_name} DEPENDS ${in_name})
  set_property(SOURCE harness.cc APPEND PROPERTY OBJECT_DEPENDS ${out_name})
endfunction(add_shader)

# shaders we want to compile

add_shader(shaders/shadertoy.vert shaders/shadertoy.spv)
add_shader(shaders/seascape.frag shaders/seascape.spv)
add_shader(shaders/city.frag shaders/city.spv)
add_shader(shaders/cartoon.frag shaders/cartoon.spv)
#add_shader(shaders/river.frag shaders/river.spv)



include_directories($ENV{VULKAN_SDK}/include)
LINK_DIRECTORIES($ENV{VULKAN_SDK}/lib)

SET(CMAKE_CXX_FLAGS "-std=c++11") 

add_executable(harness harness.cc)

target_link_libraries(harness ${GLFW_LIBRARIES} ${X11_LIBRARIES} 
  libXrandr.so libXi.so libXinerama.so libXcursor.so libvulkan.so libdl.so libpthread.so)
