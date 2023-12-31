/**
 * @file       main.cpp
 * @date       27.12.2023
 * @author     Andrei Tokmakov
 * @version    1.0
 * @copyright  Your copyright notice
 * @brief      Booking service
 */

#include <iostream>
#include <sstream>
#include <future>

#include "BookingService.h"
#include "CLI.h"

/**
 * @mainpage Booking Service Demo Documentation
 *
 * @section intro_sec Introduction
 * C++ implementation of the most simplified service for booking seats in the cinema
 */

int main([[maybe_unused]] int argc,
         [[maybe_unused]] char** argv)
{
    Booking::BookingService service;
    service.initialize();

    CLI::SimpleCLI cli (service);
    cli.start();

    return EXIT_SUCCESS;
}
