cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
#include($ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(pico_main C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

link_directories($ENV{micro_ROS_SDK_PATH}/libmicroros)
#link_directories(${CMAKE_CURRENT_LIST_DIR}/drv)

add_executable(pico_main
    main.c
    $ENV{micro_ROS_SDK_PATH}/pico_uart_transport.c
    ${CMAKE_CURRENT_LIST_DIR}/drv/motor_control.c
    ${CMAKE_CURRENT_LIST_DIR}/drv/remote_ppm.c
    ${CMAKE_CURRENT_LIST_DIR}/drv/debug_module.c
    ${CMAKE_CURRENT_LIST_DIR}/drv/cooneo_motor_control.c
    ${CMAKE_CURRENT_LIST_DIR}/drv/pwm_module.c

)
target_link_libraries(pico_main
    pico_stdlib
    microros
    hardware_pio
    hardware_pwm
    pico_multicore
    hardware_adc
 #   FreeRTOS-Kernel 
 #   FreeRTOS-Kernel-Heap4
)

pico_generate_pio_header(pico_main $ENV{pico_examples_PATH}/pio/quadrature_encoder/quadrature_encoder.pio)
target_include_directories(pico_main PUBLIC
    $ENV{micro_ROS_SDK_PATH}/libmicroros/include
    $ENV{micro_ROS_SDK_PATH}
    ${CMAKE_CURRENT_LIST_DIR}
    #${CMAKE_CURRENT_LIST_DIR}/drv
)

SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")

# Configure Pico
pico_enable_stdio_usb(pico_main 1)
pico_enable_stdio_uart(pico_main 0)
add_compile_definitions(PICO_UART_ENABLE_CRLF_SUPPORT=0)
add_compile_definitions(PICO_STDIO_ENABLE_CRLF_SUPPORT=0)
add_compile_definitions(PICO_STDIO_DEFAULT_CRLF=0)

# Generate UF2
pico_add_extra_outputs(pico_main)
