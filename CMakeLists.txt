cmake_minimum_required(VERSION 3.15.0)

project(front_panel)

enable_language(C CXX)

set(CMAKE_C_STANDARD 99)

set(PROJECT_FILES
        drivers/Inc/k32lb11.h
        main.cpp
        startup.c
        )

set(EXECUTABLE ${PROJECT_NAME}.elf)

add_executable(${EXECUTABLE} ${PROJECT_FILES})


target_include_directories(${EXECUTABLE} PRIVATE)

set(MACH cortex-m0plus)

target_compile_options(${EXECUTABLE} PRIVATE
        -mcpu=${MACH}
        -mthumb
        -mfloat-abi=soft
        -Wall
        -fno-exceptions
        $<$<CONFIG:Debug>:-O0>
        )

target_link_options(${EXECUTABLE} PRIVATE
        -T ${CMAKE_SOURCE_DIR}/linker.ld
        -mcpu=${MACH}
        -mthumb
        -mfloat-abi=soft
        --specs=nosys.specs
        -Wl,-Map=${PROJECT_NAME}.map
        )