cmake_minimum_required(VERSION 3.7.2)

project(GPIOLib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

#--------------------------------------------------------------------
#--- Collecting all files
#--------------------------------------------------------------------
set(FILES_GPIO
    GPIO/Pin.cpp
    GPIO/Pin.h
    GPIO/Led.cpp
    GPIO/Led.h
    GPIO/Sensors.cpp
    GPIO/Sensors.h)
source_group("GPIO" FILES ${FILES_GPIO})

#--------------------------------------------------------------------
#--- Setup Library
#--------------------------------------------------------------------
add_library(GPIOLib STATIC ${FILES_GPIO})

set_target_properties(GPIOLib PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(GPIOLib PUBLIC .)
