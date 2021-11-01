aux_source_directory(. DIR_LCD_SRCS)

add_library(touchlcd ${DIR_LCD_SRCS})

target_link_libraries(touchlcd PUBLIC pico_stdlib hardware_spi)
