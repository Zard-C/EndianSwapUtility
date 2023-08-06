cmake_minimum_required(VERSION 3.15)
project(endian_swap_utility)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)

include_directories(
    include
)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()

add_subdirectory(example)

install(DIRECTORY include/ DESTINATION include)


