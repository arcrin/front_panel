cmake_minimum_required(VERSION 3.15.0)

project(front_panel)

enable_language(C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_FILES
        drivers/Inc/k32lb11.h
        drivers/Src/k32lb11.cpp
        drivers/Inc/gpio_driver.h
        drivers/Src/gpio_driver.cpp
        drivers/Inc/port_driver.h
        drivers/Src/port_driver.cpp
        drivers/Inc/spi_driver.h
        drivers/Src/spi_driver.cpp
        drivers/Inc/i2c_driver.h
        drivers/Src/i2c_driver.cpp
        Src/exp7_i2c_master_rx.cpp
        startup.c
        )

set(EXECUTABLE ${PROJECT_NAME}.elf)

add_executable(${EXECUTABLE} ${PROJECT_FILES})


target_include_directories(${EXECUTABLE} PRIVATE
        drivers/Inc)

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