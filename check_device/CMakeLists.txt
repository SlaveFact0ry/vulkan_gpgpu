cmake_minimum_required(VERSION 3.10)
project(check_device)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

add_executable(check_device check_device.cpp)
target_link_libraries(check_device Vulkan::Vulkan)
