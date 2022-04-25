cmake_minimum_required(VERSION 3.20)
project(front_panel)

enable_language(C CXX)

set(CMAKE_C_STANDARD 99)

set(PROJECT_FILES
        main.cpp
        )

set(EXECUTABLE ${PROJECT_NAME}.elf)

add_executable(${EXECUTABLE} ${PROJECT_FILES})


set(MACH cortex-m0)

target_compile_options(${EXECUTABLE} PRIVATE
        -mcpu=${MACH}
        -mthumb
        -mfloat-abi=soft
        -Wall
        -fno-exceptions
        $<$<CONFIG:Debug>:-o0>
        )
