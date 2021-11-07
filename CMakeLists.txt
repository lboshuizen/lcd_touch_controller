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
        screen.cpp screen.h LcdDriver.cpp LcdDriver.h LcdScreen.cpp LcdScreen.h View.cpp View.h Gui.cpp Gui.h)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(touchcontroller pico_stdlib touchlcd hardware_spi)

# create map/bin/hex file etc.
pico_add_extra_outputs(touchcontroller)

# add url via pico_set_program_url
# example_auto_set_url(blink)
