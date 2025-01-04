# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PICOTOOL_FORCE_FETCH_FROM_GIT 1)
set(PICO_BOARD pico2)

include(pico-sdk/pico_sdk_init.cmake)
project(basilisk C CXX ASM)

pico_sdk_init()

add_executable(basilisk
    src/main.c

    src/basilisk/hardware/sn74hc595n.c
    src/basilisk/hardware/at28c64b.c
    src/basilisk/hardware/at28c64b_hidden.c
    src/basilisk/display.c
    src/basilisk/terminal.c
)

pico_set_program_name(basilisk "basilisk")
pico_set_program_version(basilisk "0.1")

pico_enable_stdio_uart(basilisk 0)
pico_enable_stdio_usb(basilisk 1)

target_include_directories(basilisk PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ./include
    ./include/basilisk
    ./include/basilisk/hardware
)

target_link_libraries(basilisk 
    pico_stdlib
    pico_rand
    hardware_gpio
)

# Add compile options
target_compile_options(basilisk PRIVATE -Wall -O3)

pico_add_extra_outputs(basilisk)
