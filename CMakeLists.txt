cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico_micro_ros_example C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD pico_w CACHE STRING "Board type")

pico_sdk_init()

link_directories(libmicroros)
add_executable(pico_micro_ros_example
    pico_micro_ros_example.c
    pico_wifi_transport.c
    ${CMAKE_CURRENT_LIST_DIR}/drv/remote_ppm.c
    ${CMAKE_CURRENT_LIST_DIR}/drv/debug_module.c
    ${CMAKE_CURRENT_LIST_DIR}/drv/cooneo_motor_control.c
    ${CMAKE_CURRENT_LIST_DIR}/drv/pwm_module.c

)
target_link_libraries(pico_micro_ros_example
    pico_stdlib
    microros
    hardware_pio
    hardware_pwm
    pico_multicore
    hardware_adc
)

pico_generate_pio_header(pico_micro_ros_example $ENV{pico_examples_PATH}/pio/quadrature_encoder/quadrature_encoder.pio)


target_include_directories(pico_micro_ros_example PUBLIC
    $ENV{micro_ROS_SDK_PATH}/libmicroros/include
    $ENV{micro_ROS_SDK_PATH}
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/..
)

target_link_libraries(pico_micro_ros_example
    pico_cyw43_arch_lwip_threadsafe_background
)

SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")

# Configure Pico
pico_enable_stdio_usb(pico_micro_ros_example 1)
pico_enable_stdio_uart(pico_micro_ros_example 0)
add_compile_definitions(PICO_UART_ENABLE_CRLF_SUPPORT=0)
add_compile_definitions(PICO_STDIO_ENABLE_CRLF_SUPPORT=0)
add_compile_definitions(PICO_STDIO_DEFAULT_CRLF=0)

# Generate UF2
pico_add_extra_outputs(pico_micro_ros_example)
