cmake_minimum_required(VERSION 3.10)
project(hello_vulkan)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

add_executable(hello_vulkan hello_vulkan.cpp)
target_link_libraries(hello_vulkan Vulkan::Vulkan)
