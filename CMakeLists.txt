cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(App)

include_directories(
  ${PROJECT_SOURCE_DIR}/App/public
  ${PROJECT_SOURCE_DIR}/App/private
  ${PROJECT_SOURCE_DIR}/helper/public
  ${PROJECT_SOURCE_DIR}/Engine/private
  ${PROJECT_SOURCE_DIR}/Engine/public
  )

file (GLOB all_SRCS
  "${PROJECT_SOURCE_DIR}/App/public/*.h"
  "${PROJECT_SOURCE_DIR}/Engine/public/*.h"
  "${PROJECT_SOURCE_DIR}/helper/public/*.h"
  "${PROJECT_SOURCE_DIR}/App/private/*.cpp"
  "${PROJECT_SOURCE_DIR}/Engine/private/*.cpp"
  )



if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  option (TEST1 "build test(or not)" ON)

  if (TEST1)
    add_library(example_test ${all_SRCS})
    enable_testing()
    add_subdirectory(test)
  endif()
endif()


add_executable(App WIN32 
              main.cpp
              ${all_SRCS}
              )