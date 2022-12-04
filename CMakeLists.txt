cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico_micro_ros_motor_control C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

link_directories(libmicroros)
add_executable(pico_micro_ros_motor_control
    pico_micro_ros_motor_control.c
    pico_uart_transport.c
)
target_link_libraries(pico_micro_ros_motor_control
    pico_stdlib
    microros
    pico_multicore
    hardware_pio
    hardware_pwm
)

pico_generate_pio_header(pico_micro_ros_motor_control ${CMAKE_CURRENT_LIST_DIR}/quadrature_encoder.pio)

target_include_directories(pico_micro_ros_motor_control PUBLIC
    libmicroros/include
)

target_include_directories(pico_micro_ros_motor_control PUBLIC
    Motor/
)

SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")

# Configure Pico
pico_enable_stdio_usb(pico_micro_ros_motor_control 1)
pico_enable_stdio_uart(pico_micro_ros_motor_control 0)
add_compile_definitions(PICO_UART_ENABLE_CRLF_SUPPORT=0)
add_compile_definitions(PICO_STDIO_ENABLE_CRLF_SUPPORT=0)
add_compile_definitions(PICO_STDIO_DEFAULT_CRLF=0)

# Generate UF2
pico_add_extra_outputs(pico_micro_ros_motor_control)
