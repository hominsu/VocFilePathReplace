cmake_minimum_required(VERSION 3.19)
project(VocFilePathReplace)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXE_LINKER_FLAGS -static)

file(GLOB ProjectSRC
        "*.cc"
        "*.h")

find_package(Threads REQUIRED)

add_executable(VocFilePathReplace ${ProjectSRC})

target_link_libraries(VocFilePathReplace Threads::Threads)