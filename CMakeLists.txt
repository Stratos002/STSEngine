cmake_minimum_required(VERSION 3.18)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

project(STSEngine VERSION 1.0)

find_package(Vulkan REQUIRED)

add_subdirectory(libs/glfw)

file(GLOB_RECURSE SRC_FILES src/*.c)
add_executable(STSEngine ${SRC_FILES})
target_include_directories(STSEngine PUBLIC include/ libs/vulkan/ libs/volk/ libs/glfw/include)
add_definitions(-DSTSE_PLATFORM_LINUX -DSTSE_GRAPHICS_API_VULKAN -DSTSE_CONFIGURATION_DEBUG)
target_compile_options(STSEngine PRIVATE -Werror -Wall -Wextra -pedantic)
target_link_libraries(STSEngine glfw)