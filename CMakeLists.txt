cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(motor_encoder_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(motor_encoder
    motor_encoder.c
)

add_executable(motor_pwm
    motor_pwm.c
)

add_executable(motor_adc
	motor_adc.c
)

add_executable(motor_control
	motor_control.c
)

target_link_libraries(motor_adc pico_stdlib pico_time pico_unique_id hardware_adc hardware_pwm)
target_link_libraries(motor_encoder pico_stdlib pico_time pico_unique_id hardware_adc hardware_pwm)
target_link_libraries(motor_pwm pico_stdlib hardware_adc hardware_pwm)
target_link_libraries(motor_control pico_stdlib pico_time pico_unique_id hardware_adc hardware_pwm)

pico_enable_stdio_usb(motor_encoder 1)
pico_enable_stdio_uart(motor_encoder 0)

pico_enable_stdio_usb(motor_pwm 1)
pico_enable_stdio_uart(motor_pwm 0)

pico_enable_stdio_usb(motor_adc 1)
pico_enable_stdio_uart(motor_adc 0)

pico_enable_stdio_usb(motor_control 1)
pico_enable_stdio_uart(motor_control 0)

pico_add_extra_outputs(motor_encoder)
pico_add_extra_outputs(motor_pwm)
pico_add_extra_outputs(motor_adc)
pico_add_extra_outputs(motor_control)
