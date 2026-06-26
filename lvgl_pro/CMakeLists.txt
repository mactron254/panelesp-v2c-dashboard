# Generated-style file for LVGL Pro Editor export.
# ESP-IDF firmware build does not use this folder directly yet.

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  cmake_minimum_required(VERSION 3.10)
  project(LVGLProject)
  set(IS_TOP_LEVEL TRUE)
else()
  set(IS_TOP_LEVEL FALSE)
endif()

if(DEFINED LVGL_COMPONENT_LIB_NAME)
  set(LIB_NAME ${LVGL_COMPONENT_LIB_NAME})
else()
  set(LIB_NAME lib-panelesp-ui)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/file_list_gen.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/component_lib_list_gen.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/user_config.cmake)

add_library(${LIB_NAME} ${LV_EDITOR_PROJECT_SOURCES})

if(DEFINED LV_EDITOR_COMPONENT_LIB_LIST)
  target_link_libraries(${LIB_NAME} PUBLIC ${LV_EDITOR_COMPONENT_LIB_LIST})
endif()

if(TARGET lvgl)
  target_link_libraries(${LIB_NAME} PUBLIC lvgl)
endif()

target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(DEFINED COMMON_INCLUDE_DIRS)
  target_include_directories(${LIB_NAME} PRIVATE ${COMMON_INCLUDE_DIRS})
endif()
