/**============================================================================
Name        : main.cpp
Created on  : 27.12.2023
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : Booking service
============================================================================**/

#include <iostream>
#include <string_view>
#include <vector>
#include <sstream>
#include <future>

#include "BookingService.h"
#include "CLI.h"
#include "WebService.h"


namespace Tests
{
    using namespace Booking;

    Booking::BookingService service;

    void getAllMoviesTest()
    {
        service.initialize();

        std::vector<Movie> movies = service.getAllPlayingMovies();
        for (const Movie& m: movies)
            std::cout << m << std::endl;
    }

    void getAllTheatersTest()
    {
        service.initialize();

        std::vector<Theater> theaters = service.getTheaters();
        for (const Theater& m: theaters)
            std::cout << m << std::endl;
    }

    void getSeatsAvailableTest()
    {
        service.initialize();

        std::vector<uint16_t> seats = service.getSeatsAvailable( {"4DX"}, Movie{"Fight Club"});
        std::cout <<seats << std::endl;
    }

    void getSeatsAvailableAfterBookingTest()
    {
        service.initialize();
        auto premiere = service.getPremiere({"4DX"}, {"Fight Club"});
        if (!premiere.has_value())
            return;

        std::cout << premiere.value()->getSeatsAvailable() << std::endl;
        std::cout << std::boolalpha << premiere.value()->bookSeats({1,2,3}) << std::endl;
        std::cout << premiere.value()->getSeatsAvailable() << std::endl;
        std::cout << std::boolalpha << premiere.value()->bookSeats({1,2,3}) << std::endl;
        std::cout << premiere.value()->getSeatsAvailable() << std::endl;
        std::cout << std::boolalpha << premiere.value()->bookSeats({4, 5, 20}) << std::endl;
        std::cout << premiere.value()->getSeatsAvailable() << std::endl;
    }

    void getAllScheduleTest()
    {
        service.initialize();
        for (const auto & premiere: service.bookingSchedule)
            std::cout << premiere << std::endl;
    }
}


int main([[maybe_unused]] int argc,
         [[maybe_unused]] char** argv)
{
    using namespace Booking;

    // Tests::getAllMoviesTest();
    // Tests::getAllTheatersTest();
    // Tests::getSeatsAvailableTest();
    // Tests::getSeatsAvailableAfterBookingTest();
    // Tests::getAllScheduleTest();

    // WebService::start(argc, argv);


    Booking::BookingService service;
    service.initialize();

    CLI::SimpleCLI cli (service);
    cli.start();

    return EXIT_SUCCESS;
}
