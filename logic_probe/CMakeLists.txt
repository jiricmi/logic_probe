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
  PRIVATE Core/Src/ansi/ansi_display.c
          Core/Src/ansi/ansi_abstraction_layer.c
          Core/Src/ansi/ansi_pages.c
          Core/Src/ansi/ansi_ascii_text.c
          Core/Src/measure/adc_control.c
          Core/Src/utils.c
          Core/Src/callbacks.c
          Core/Src/uart_control.c
          Core/Src/measure_tools.c
          Core/Src/my_error_handle.c)

target_include_directories(
  ${CMAKE_PROJECT_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/Core/Inc/ansi
  PRIVATE ${CMAKE_SOURCE_DIR}/Core/Inc/measure)

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
endif()
