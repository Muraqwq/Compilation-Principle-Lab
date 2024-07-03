cmake_minimum_required(VERSION 3.20)

project("Parser_Frontend")
include_directories(./header)

aux_source_directory(./src DIR_SRCS)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(Parser_Frontend "main.cpp" ${DIR_SRCS})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Parser_Frontend PROPERTY CXX_STANDARD 20)
endif()