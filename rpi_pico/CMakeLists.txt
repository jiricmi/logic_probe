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
  ansi/ansi_pages.c
  probe_error.c
  global_vars.c)

target_include_directories(
  ${CMAKE_PROJECT_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/ansi
  PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(logic_probe pico_stdlib pico_multicore)

pico_enable_stdio_usb(logic_probe 1)
pico_enable_stdio_uart(logic_probe 0)

pico_add_extra_outputs(logic_probe)
