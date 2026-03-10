cmake_minimum_required(VERSION 3.20)
project(stepvr_driver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT WIN32)
  message(FATAL_ERROR "This project currently targets Windows only.")
endif()

if(NOT DEFINED ENV{OPENVR_SDK_ROOT})
  message(FATAL_ERROR "Please set OPENVR_SDK_ROOT to your local OpenVR SDK checkout.")
endif()

set(OPENVR_SDK_ROOT $ENV{OPENVR_SDK_ROOT})
set(OPENVR_HEADERS ${OPENVR_SDK_ROOT}/headers)
set(OPENVR_LIB ${OPENVR_SDK_ROOT}/lib/win64/openvr_api.lib)

add_library(driver_stepvr SHARED
  src/driver_main.cpp
  src/step_provider.cpp
  src/step_controller.cpp
  src/bridge_udp.cpp
)

target_include_directories(driver_stepvr PRIVATE
  ${OPENVR_HEADERS}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(driver_stepvr PRIVATE
  ws2_32
  ${OPENVR_LIB}
)

target_compile_definitions(driver_stepvr PRIVATE
  WIN32_LEAN_AND_MEAN
  NOMINMAX
)

set_target_properties(driver_stepvr PROPERTIES
  OUTPUT_NAME "driver_stepvr"
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/win64
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/win64
)

add_custom_command(TARGET driver_stepvr POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/resources/input
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/resources/localization
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/resources/settings
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/driver.vrdrivermanifest ${CMAKE_BINARY_DIR}/driver.vrdrivermanifest
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/resources/input/stepvr_profile.json ${CMAKE_BINARY_DIR}/resources/input/stepvr_profile.json
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/resources/input/legacy_binding_stepvr.json ${CMAKE_BINARY_DIR}/resources/input/legacy_binding_stepvr.json
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/resources/input/stepvr.svg ${CMAKE_BINARY_DIR}/resources/input/stepvr.svg
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/resources/localization/localization.json ${CMAKE_BINARY_DIR}/resources/localization/localization.json
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/resources/settings/default.vrsettings ${CMAKE_BINARY_DIR}/resources/settings/default.vrsettings
)