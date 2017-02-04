cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_CXX_COMPILER "g++-6")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Portaudio REQUIRED)

add_subdirectory(.. GPIOLib-binary)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PORTAUDIO_DEFINITIONS}" )


##  --------------------------------------------------------------------------------------
##  LED
##  --------------------------------------------------------------------------------------

project(led)
add_executable(led led.cpp)

target_include_directories(led PUBLIC . ${PORTAUDIO_INCLUDE_DIRS})
target_link_libraries(led PUBLIC GPIOLib ${PORTAUDIO_LIBRARIES})

##  --------------------------------------------------------------------------------------
##  RGB LED
##  --------------------------------------------------------------------------------------

project(rgbled)
add_executable(rgbled led_rgb.cpp)

target_include_directories(rgbled PUBLIC . ${PORTAUDIO_INCLUDE_DIRS})
target_link_libraries(rgbled PUBLIC GPIOLib ${PORTAUDIO_LIBRARIES})

