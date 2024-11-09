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
          Core/Src/utils.c
          Core/Src/callbacks.c
          Core/Src/uart_control.c
          Core/Src/measure_tools.c)

target_include_directories(${CMAKE_PROJECT_NAME}
                           PRIVATE ${CMAKE_SOURCE_DIR}/Core/Inc/ansi)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE)

target_link_libraries(${CMAKE_PROJECT_NAME} stm32cubemx)
