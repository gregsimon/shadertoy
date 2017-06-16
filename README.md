# Shaderharness

Basic harness for running shaders from ShaderToy on a Vulkan API. Note the shaders don't come over for free; they must be "reworked" as vulkan fragment shaders. This involves

1. Adding a header to the top indicating it's a vulkan shader:
```
#version 450
#extension GL_ARB_separate_shader_objects : enable
```
2. Adding the shader inputs themselves. Right now we are patching these via a UBO:
```
layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 iResolution;
    float iGlobalTime;
    vec3 iMouse;
} ubo;

 layout(location = 0) out vec4 outColor;
 ```
 3. Updating the input variable names (iResolution -> ubo.iResolution)
 4. Creating a main() at the bottom that calls mainImage():
 ```
 void main() {
  vec4 color = vec4(0.0,0.0,0.0,1.0);
  mainImage(color, gl_FragCoord.xy);
  color.w = 1.0;
  outColor = color;
}
 ```

## TODO

- make the shader inputs more compatible looking so less code in the ported shaders has to change.

# Building

You will need to build glfw for your platform. Best is to grab the source and compile it.

```
git clone https://github.com/glfw/glfw.git
cd glfw
cmake .
make

```