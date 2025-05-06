cmake_minimum_required(VERSION 3.31)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(logic_probe)

pico_sdk_init()

add_executable(
  logic_probe
  main.c
  utils.c
  ansi/ansi_abstraction_layer.c
  ansi/ansi_ascii_text.c
  ansi/pages/ansi_page_main.c
  ansi/pages/ansi_page_voltage_measure.c
  ansi/pages/ansi_page_frequency_reader.c
  ansi/pages/ansi_page_levels.c
  ansi/ansi_pages.c
  perif/uart_control/uart_control.c
  perif/uart_control/control_voltage_measure.c
  perif/uart_control/control_frequency_reader.c
  perif/uart_control/control_levels.c
  measure/adc_control.c
  measure/signal_detection.c
  measure/levels.c
  probe_usb.c
  probe_error.c
  global_vars.c
  loop.c)

target_include_directories(
  ${CMAKE_PROJECT_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/ansi
  PRIVATE ${CMAKE_SOURCE_DIR}/measure
  PRIVATE ${CMAKE_SOURCE_DIR}/perif/uart_control
  PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(logic_probe pico_stdlib pico_multicore hardware_adc
                      hardware_dma hardware_pwm)

pico_enable_stdio_usb(logic_probe 1)
pico_enable_stdio_uart(logic_probe 0)

pico_add_extra_outputs(logic_probe)
