# hello

cmake_minimum_required(VERSION 3.7)

include_directories($ENV{VULKAN_SDK}/include)

add_executable(shaderharness harness.cc)

