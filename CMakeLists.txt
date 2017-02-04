cmake_minimum_required(VERSION 2.8.11)

project(GPIOLib)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#--------------------------------------------------------------------
#--- Collecting all files
#--------------------------------------------------------------------

file(GLOB FILES_GPIO RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/GPIOLib/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/GPIOLib/*.h)
source_group("GPIOLib" FILES ${FILES_GPIO})

#--------------------------------------------------------------------
#--- Setup Library
#--------------------------------------------------------------------

add_library(GPIOLib STATIC ${FILES_GPIO})

set_target_properties(GPIOLib PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(GPIOLib PUBLIC .)
