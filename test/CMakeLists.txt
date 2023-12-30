cmake_minimum_required(VERSION 3.16.3 FATAL_ERROR)

project(tests)
add_compile_options(-c -Wall -Werror -Wextra -O3 -std=c++2a)

if (WIN32)
    message(STATUS "Building under Widndow OS")
    # Set up correct path to Boost Library folder
    set(BOOST_ROOT "C:/boost_1_78_0/")
    set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include")
    set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")
    set(Boost_USE_STATIC_LIBS ON)
else(UNIX)
    message(STATUS "Building under NIX* OS")
endif()

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

if (NOT Boost_FOUND)
    message(FATAL "Boost not found")
else()
    message(STATUS "Boost version: Boost_VERSION=${Boost_VERSION}")
endif()

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
include_directories(${SRC_DIR})

# include all components
add_executable(tests
        main.cpp
        ${SRC_DIR}/BookingService.h
        ${SRC_DIR}/BookingService.cpp
        ${SRC_DIR}/CLI.cpp
        ${SRC_DIR}/CLI.h
        ${SRC_DIR}/Database.cpp
        ${SRC_DIR}/Database.h
        )

TARGET_LINK_LIBRARIES(tests boost_unit_test_framework)
