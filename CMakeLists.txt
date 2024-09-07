cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)

project(STSEngine VERSION 1.0)

file(GLOB_RECURSE SRC_FILES src/*.c)
add_executable(STSEngine ${SRC_FILES})
target_include_directories(STSEngine PUBLIC include/ libs/vulkan/ libs/volk/)
add_definitions(-DSTSE_USE_VULKAN -DSTSE_CONFIGURATION_DEBUG)
target_compile_options(STSEngine PRIVATE -Werror -Wall -Wextra -pedantic)
