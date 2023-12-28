cmake_minimum_required(VERSION 3.25)
project(BookingService)

set(CMAKE_PREFIX_PATH ../third_party/poco/cmake-build)

add_compile_options(-c -Wall -Wextra -O3 -std=c++2a)

find_package(Poco REQUIRED Foundation Net Util)

set(CMAKE_CXX_STANDARD 23)


add_executable(BookingService
        main.cpp
        BookingService.cpp BookingService.h
        CLI.cpp CLI.h
        WebService.cpp WebService.h
)

TARGET_LINK_LIBRARIES(
        BookingService
        Poco::Foundation Poco::Net Poco::Util
        pthread
)