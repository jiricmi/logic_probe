cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_PROJECT_NAME logic_probe)

include("cmake/gcc-arm-none-eabi.cmake")

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

enable_language(C ASM)

project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

add_executable(${CMAKE_PROJECT_NAME})

add_subdirectory(cmake/stm32cubemx)

target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE)

target_sources(
  ${CMAKE_PROJECT_NAME}
  PRIVATE Core/Src/ansi/ansi_abstraction_layer.c
          Core/Src/ansi/ansi_pages.c
          Core/Src/ansi/ansi_ascii_text.c
          Core/Src/ansi/pages/ansi_page_main.c
          Core/Src/ansi/pages/ansi_page_voltage_measure.c
          Core/Src/ansi/pages/ansi_page_shift_register.c
          Core/Src/ansi/pages/ansi_page_neopixel_measure.c
          Core/Src/ansi/pages/ansi_page_frequency_reader.c
          Core/Src/ansi/pages/ansi_page_impulse_generator.c
          Core/Src/ansi/pages/ansi_page_uart.c
          Core/Src/measure/adc_control.c
          Core/Src/measure/signal_detector.c
          Core/Src/measure/signal_generator.c
          Core/Src/measure/tim_setup.c
          Core/Src/measure/advanced/neopixel.c
          Core/Src/measure/advanced/shift_register.c
          Core/Src/measure/advanced/uart.c
          Core/Src/utils.c
          Core/Src/callbacks.c
          Core/Src/perif/outputs/visual_output.c
          Core/Src/perif/outputs/gpio_outputs.c
          Core/Src/perif/inputs/extern_button.c
          Core/Src/perif/uart_control/uart_control.c
          Core/Src/perif/uart_control/control_shift_register.c
          Core/Src/perif/uart_control/control_neopixel_measure.c
          Core/Src/perif/uart_control/control_voltage_measure.c
          Core/Src/perif/uart_control/control_impulse_generator.c
          Core/Src/perif/uart_control/control_frequency_reader.c
          Core/Src/perif/uart_control/control_uart_measure.c
          Core/Src/my_error_handle.c
          Core/Src/loop.c)

target_include_directories(
  ${CMAKE_PROJECT_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/Core/Inc/ansi
  PRIVATE ${CMAKE_SOURCE_DIR}/Core/Inc/ansi/pages
  PRIVATE ${CMAKE_SOURCE_DIR}/Core/Inc/measure
  PRIVATE ${CMAKE_SOURCE_DIR}/Core/Inc/perif
  PRIVATE ${CMAKE_SOURCE_DIR}/Core/Inc/perif/inputs
  PRIVATE ${CMAKE_SOURCE_DIR}/Core/Inc/perif/outputs
  PRIVATE ${CMAKE_SOURCE_DIR}/Core/Inc/perif/uart_control
  PRIVATE ${CMAKE_SOURCE_DIR}/Core/Inc)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE)

target_link_libraries(${CMAKE_PROJECT_NAME} stm32cubemx)

# Add compiler flags
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  target_compile_options(
    ${CMAKE_PROJECT_NAME}
    PRIVATE -Wall # Enable all warnings
            -Wextra # Enable extra warnings
            -Wpedantic # Enable pedantic checks
            -Wshadow # Warn if a variable is shadowing another
  )

  # Size optimization for Release mode
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Os -DSOP8)

    add_custom_command(
      TARGET ${CMAKE_PROJECT_NAME}
      POST_BUILD
      COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
              ${CMAKE_PROJECT_NAME}.hex)

    # Convert to bin file -> add conditional check?
    add_custom_command(
      TARGET ${CMAKE_PROJECT_NAME}
      POST_BUILD
      COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
              ${CMAKE_PROJECT_NAME}.bin)
  endif()
endif()
