# VMCE

VMCE (Vulkan MCE) is the vulkan support for MCE.

## Installation
To compile the project
```
cmake -S . -B build
cmake --build ./build/
```

To compile on debug mode
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build ./build/
```

The compilation generate 2 folders:
- ``bin`` that contain all binaries compiled
- ``lib`` that contain all libraries compiled

## Requirements
To compile the VMCE library you need to have theses libraries
- [Vulkan](https://www.vulkan.org/)
- [GLFW](https://www.glfw.org/)
