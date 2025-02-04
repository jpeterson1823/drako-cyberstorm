# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_BUILD_TYPE MinSizeRel)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PICOTOOL_FORCE_FETCH_FROM_GIT 1)
set(PICO_BOARD pico2)

include(pico-sdk/pico_sdk_init.cmake)
project(drako C CXX ASM)

pico_sdk_init()

add_executable(drako
    src/main.c

    src/drako/hardware/sn74hc595n.c
    src/drako/hardware/at28c64b.c
    src/drako/hardware/at28c64b_hidden.c

    src/drako/modules/display.c
    src/drako/modules/terminal.c
    src/drako/commands.c

    src/drako/tests.c
)

pico_set_program_name(drako "drako")
pico_set_program_version(drako "0.1")

pico_enable_stdio_uart(drako 0)
pico_enable_stdio_usb(drako 1)

target_include_directories(drako PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ./include
    ./include/drako
    ./include/drako/hardware
)

target_link_libraries(drako 
    pico_stdlib
    pico_rand
    hardware_gpio
)

# Add compile options -- unneccessary with MinSizeRel set
#target_compile_options(drako PRIVATE -Wall -Os -s -fno-ident -fno-asynchronous-unwind-tables)

pico_add_extra_outputs(drako)
