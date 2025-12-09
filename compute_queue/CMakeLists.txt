cmake_minimum_required(VERSION 3.10)
project(create_queue)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

add_executable(create_queue create_queue.cpp)
target_link_libraries(create_queue Vulkan::Vulkan)
