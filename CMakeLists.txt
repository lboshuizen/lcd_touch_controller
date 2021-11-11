cmake_minimum_required(VERSION 3.20)

include(pico_sdk_import.cmake)

project(touchcontroller C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_subdirectory(lib/lcd)
include_directories(./lib/lcd)

add_executable(
        touchcontroller
        main.cpp
        lcd_test.c
        LcdDriver.cpp LcdDriver.h LcdScreen.cpp LcdScreen.h Gui.cpp Gui.h TouchDriver.cpp TouchDriver.h Spi.cpp Spi.h Color.cpp Color.h View.cpp View.h)

pico_enable_stdio_usb(touchcontroller 1)
pico_enable_stdio_uart(touchcontroller 0)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(touchcontroller pico_stdlib touchlcd hardware_spi)

# create map/bin/hex file etc.
pico_add_extra_outputs(touchcontroller)

# add url via pico_set_program_url
# example_auto_set_url(scan)
