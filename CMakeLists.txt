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
        drivers/Inc/adc_driver.h
        drivers/Src/adc_driver.cpp
#        drivers/Inc/uart_driver.h
#        drivers/Src/uart_driver.cpp
        drivers/Inc/lpuart_driver.h
        drivers/Src/lpuart_driver.cpp
        drivers/Inc/lptmr_driver.h
        drivers/Src/lptmr_driver.cpp
        drivers/Inc/tpm_driver.h
        drivers/Src/tpm_driver.cpp
        modules/Inc/led_control.h
        modules/Src/led_control.cpp
        modules/Inc/front_panel_peripheral.h
        modules/Src/front_panel_peripheral.cpp
#        modules/fonts/fonts.h
#        modules/fonts/font8.cpp
#        modules/fonts/font12.cpp
#        modules/fonts/font16.cpp
#        modules/fonts/font20.cpp
#        modules/fonts/font24.cpp
        modules/Inc/GUI_Paint.h
        modules/Src/GUI_Paint.cpp
#        modules/ImageData.h
#        modules/ImageData.cpp
        modules/Inc/epaper_front_panel.h
        modules/Src/epaper_front_panel.cpp
        Src/front_panel_controller.cpp
        startup.c
        )

set(EXECUTABLE ${PROJECT_NAME}.elf)

add_executable(${EXECUTABLE} ${PROJECT_FILES})


target_include_directories(${EXECUTABLE} PRIVATE
        drivers/Inc
        drivers/Src
        modules
        modules/Inc
        modules/Src)

set(MACH cortex-m0plus)

target_compile_options(${EXECUTABLE} PRIVATE
        -mcpu=${MACH}
        -mthumb
        -mfloat-abi=soft
        -Wall
        -fno-exceptions
        $<$<CONFIG:Debug>:-O1>
        )

target_link_options(${EXECUTABLE} PRIVATE
        -T ${CMAKE_SOURCE_DIR}/linker.ld
        -mcpu=${MACH}
        -mthumb
        -mfloat-abi=soft
        --specs=nosys.specs
        -Wl,-Map=${PROJECT_NAME}.map
        )

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
        COMMAND arm-none-eabi-objcopy -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
        COMMAND arm-none-eabi-objcopy -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
        COMMAND arm-none-eabi-nm -C -l -n -S ${PROJECT_NAME}.elf > ${PROJECT_NAME}.sym
        COMMAND arm-none-eabi-nm -C -l --size-sort -S ${PROJECT_NAME}.elf > ${PROJECT_NAME}.symsize
        COMMAND arm-none-eabi-objdump -S -C -j .text -j .data -j .bss ${PROJECT_NAME}.elf > ${PROJECT_NAME}.lst
        COMMAND arm-none-eabi-size ${PROJECT_NAME}.elf | tee ${PROJECT_NAME}.siz
        WORKING_DIRECTORY ${OUTPUT_DIR})