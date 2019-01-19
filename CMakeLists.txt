cmake_minimum_required(VERSION 2.8.4)


#------------------------------------------------------------------------------
# read local.properties file for set arduino port
#
message("Read local.properties")
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/local.properties")
    include(${CMAKE_CURRENT_SOURCE_DIR}/local.properties)
    if ("${ARDU_PORT}" STREQUAL "")
        message(FATAL_ERROR "The file local.properties are found but the variable ARDU_PORT is not set, CMake will exit.")
    else ()
        message("Arduino upload port is set to: ${ARDU_PORT}")
    endif ()
else ()
    message(FATAL_ERROR "No local.properties file found. You must create this file with content set(ARDU_PORT /dev/cu.usbserial-1420) for Mac or set(ARDU_PORT com3) for Windows! CMake will exit.")
endif ()

include_directories(Blink/libraries/StandardCplusplus)


set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_PROJECT_NAME SIGNAL_BEACON)
set(${CMAKE_PROJECT_NAME}_BOARD pro)
set(ARDUINO_CPU 8MHzatmega328)
project(${CMAKE_PROJECT_NAME})
# set(${CMAKE_PROJECT_NAME}_SRCS)
# set(${CMAKE_PROJECT_NAME}_HDRS)
# set(${CMAKE_PROJECT_NAME}_LIBS)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
set(${CMAKE_PROJECT_NAME}_SKETCH ardu_neopixel_rotating-beacon/ardu_neopixel_rotating-beacon.ino)
set(${CMAKE_PROJECT_NAME}_PROGRAMMER avrispmkii)
#set(${CMAKE_PROJECT_NAME}_PORT /dev/cu.usbserial-1420)
set(${CMAKE_PROJECT_NAME}_PORT ${ARDU_PORT})
set(${CMAKE_PROJECT_NAME}_AFLAGS -v)
set(pro.upload.speed 57600)


include_directories(Blink)
set(SOURCE_FILES
        Logger.cpp
        LoggerFactory.cpp
        TestClass.cpp
        )

set(HEADERS_FILES
        Logger.h
        LoggerFactory.h
        TestClass.h
        )
set(${CMAKE_PROJECT_NAME}_HDRS ${HEADERS_FILES})
set(${CMAKE_PROJECT_NAME}_SRCS ${SOURCE_FILES})

#generate_arduino_firmware(${CMAKE_PROJECT_NAME})
#generate_arduino_firmware(${CMAKE_PROJECT_NAME}
#        SRCS ${SOURCE_FILES}
#        HDRS ${HEADERS_FILES}
#        )


include_directories(${CMAKE_SOURCE_DIR})
file(GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/*.cpp)
file(GLOB HDR_FILES ${CMAKE_SOURCE_DIR}/*.h)

set(PROJ_SRC ${SRC_FILES})
set(PROJ_HDR ${HDR_FILES})

generate_arduino_firmware(${CMAKE_PROJECT_NAME}
        SRCS ${PROJ_SRC}
        HDRS ${PROJ_HDR}
        )