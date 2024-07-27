cmake_minimum_required(VERSION 3.14)
project(VMCE VERSION 1.0 LANGUAGES CXX)

# Compile MCE
add_subdirectory(MCE)

# Import Vulkan
find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# Define target binary destination
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# Define sources files
set(SRC_PATH src)
set(INCLUDE_PATH include)
set(
    SRC_FILES

    ${SRC_PATH}/main.cpp
    ${SRC_PATH}/application.cpp
)

# Add compilation flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wextra -Werror -fPIC")

# Include headers
include_directories(${INCLUDE_PATH})

add_library(vmce SHARED ${SRC_FILES})
target_link_libraries(vmce Vulkan::Vulkan glfw)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -DVMCE_DEBUG -g3")

# Compile tests
add_subdirectory(tests)
