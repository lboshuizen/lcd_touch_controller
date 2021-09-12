cmake_minimum_required(VERSION 3.20)

include(pico_sdk_import.cmake)

project(touchcontroller C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_subdirectory(lib/config)
add_subdirectory(lib/lcd)
add_subdirectory(lib/font)
add_subdirectory(lib/fatfs)
add_subdirectory(lib/sdcard)

include_directories(./lib/config)
include_directories(./lib/lcd)
include_directories(./lib/font)
include_directories(./lib/fatfs)
include_directories(./lib/sdcard)

add_executable(
        touchcontroller
        main.c
        lcd_test.c
        )

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(touchcontroller pico_stdlib lcd font config pico_stdlib hardware_spi fatfs)

# create map/bin/hex file etc.
pico_add_extra_outputs(touchcontroller)

# add url via pico_set_program_url
# example_auto_set_url(blink)
