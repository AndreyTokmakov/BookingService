cmake_minimum_required(VERSION 3.16.3)
project(BookingService)

add_compile_options(-c -Wall -Wextra -O3 -std=c++2a)

set(CMAKE_CXX_STANDARD 20)

add_executable(BookingService
        main.cpp
        Database.h
        BookingService.cpp BookingService.h
        CLI.cpp CLI.h
)

TARGET_LINK_LIBRARIES(
        BookingService
        pthread
)