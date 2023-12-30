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
 *
 * @section install_sec Installation
 *
 * @subsection install_dependencies Installing Dependencies
 * Do somethings ...
 *
 * @subsection install_library Installing Library
 * Do somethings ...
 *
 * @subsection install_example Installing Examples
 * Do somethings ...
 */

int main([[maybe_unused]] int argc,
         [[maybe_unused]] char** argv)
{
    using namespace Booking;

    Booking::BookingService service;
    service.initialize();

    CLI::SimpleCLI cli (service);
    cli.start();

    return EXIT_SUCCESS;
}
