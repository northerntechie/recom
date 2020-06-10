cmake_minimum_required(VERSION 3.15)

project(stripper LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)

set(CMAKE_CXX_STANDARD 20)

include_directories(src/include)

find_package(docopt 0.6 REQUIRED)

add_executable(stripper
        src/stripper.cpp)
target_link_libraries(stripper docopt)
