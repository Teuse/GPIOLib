cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Portaudio     REQUIRED)

add_subdirectory(.. GPIOLib-binary)

##  --------------------------------------------------------------------------------------
##  LED
##  --------------------------------------------------------------------------------------

message(STATUS "Portaudio found: ${PORTAUDIO_FOUND}")
message(STATUS "Portaudio compiler switches: ${PORTAUDIO_DEFINITIONS}")
message(STATUS "Portaudio include dir: ${PORTAUDIO_INCLUDE_DIRS}")
message(STATUS "Portaudio library dir: ${PORTAUDIO_LIBRARIES}")


project(led)
add_executable(led led.cpp)

target_include_directories(led PUBLIC . ${PORTAUDIO_INCLUDE_DIRS})
target_link_libraries(led PUBLIC GPIOLib ${PORTAUDIO_LIBRARIES})

if(APPLE)
  target_link_libraries(led PUBLIC "-framework AudioToolbox -framework CoreFoundation" )
elseif(WIN32)
  target_link_libraries(led PUBLIC mfplat.lib mfreadwrite.lib mfuuid.lib Propsys.lib)
elseif(UNIX)
  target_link_libraries(led PUBLIC "-lpthread -lasound -lrt") # -ljack
endif()


##  --------------------------------------------------------------------------------------
##  RGB LED
##  --------------------------------------------------------------------------------------

project(rgbled)
add_executable(rgbled led_rgb.cpp)

target_include_directories(rgbled PUBLIC . ${PORTAUDIO_INCLUDE_DIRS})
target_link_libraries(rgbled PUBLIC GPIOLib ${PORTAUDIO_LIBRARIES})

if(APPLE)
  target_link_libraries(rgbled PUBLIC "-framework AudioToolbox -framework CoreFoundation" )
elseif(WIN32)
  target_link_libraries(rgbled PUBLIC mfplat.lib mfreadwrite.lib mfuuid.lib Propsys.lib)
elseif(UNIX)
  target_link_libraries(rgbled PUBLIC "-lpthread -lasound -lrt") # -ljack
endif()
