//============================================================================
// Name        : main.cpp
// Created on  : 17.08.2021
// Author      : Tokmakov Andrey
// Version     : 1.0
// Copyright   : Your copyright notice
// Description : C++ Boost Project
//============================================================================

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MyTest


#include <iostream>
#include <memory>
#include <algorithm>

#include <thread>
#include <future>

#include <vector>
#include <algorithm>
#include <array>
#include <set>
#include <string>
#include <string_view>
#include <mutex>
#include <atomic>
#include <numeric>
#include <iomanip>

#include <boost/version.hpp>
#include <boost/test/unit_test.hpp>

#include "BookingService.h"
#include "CLI.h"
#include "WebService.h"

using namespace Booking;

void insertTestData(Booking::BookingService& service)
{
    service.addMovie("Fight Club");
    service.addMovie("Terminator");
    service.addMovie("Inception");

    service.addTheater("4DX");
    service.addTheater("Electric Cinema");
    service.addTheater("Sun Pictures");

    service.scheduleMovie("Fight Club", "4DX") ;
    service.scheduleMovie("Fight Club", "Electric Cinema");
    service.scheduleMovie("Terminator", "Sun Pictures");
    service.scheduleMovie("Terminator", "4DX");
    service.scheduleMovie("Inception", "Electric Cinema");
}

struct TestFixture
{
    Booking::BookingService service;

    TestFixture() {
        insertTestData(service);
    }

    ~TestFixture() = default;
};


BOOST_FIXTURE_TEST_CASE(TestGetAllPlayingMovies, TestFixture)
{
    std::vector<Movie> movies = service.getAllPlayingMovies();
    for (const std::string& movieName: std::vector<std::string>{"Fight Club", "Terminator", "Inception"})
    {
        const bool exits = std::any_of(movies.cbegin(), movies.cend(), [&](const auto& movie) {
            return movie.name == movieName;
        });
        BOOST_CHECK_EQUAL(exits, true);
    }
}

BOOST_FIXTURE_TEST_CASE(TestGetAlltheaters, TestFixture)
{
    const std::vector<Theater> theaters = service.getTheaters();
    for (const std::string& theaterName: std::vector<std::string>{"4DX", "Electric Cinema", "Sun Pictures"})
    {
        const bool exits = std::any_of(theaters.cbegin(), theaters.cend(), [&](const auto& theater) {
            return theater.name == theaterName;
        });
        BOOST_CHECK_EQUAL(exits, true);
    }
}

BOOST_FIXTURE_TEST_CASE(TestGetTheatersPlayingMovies, TestFixture)
{
    const Movie movie { "Fight Club" };
    const std::vector<Theater> theaters = service.getTheatersByMovie(movie);

    BOOST_CHECK_EQUAL(theaters.size(), 2);
    for (const std::string& theaterName: std::vector<std::string>{"4DX", "Electric Cinema"})
    {
        const bool exits = std::any_of(theaters.cbegin(), theaters.cend(), [&](const auto& theater) {
            return theater.name == theaterName;
        });
        BOOST_CHECK_EQUAL(exits, true);
    }
}

BOOST_FIXTURE_TEST_CASE(TestGetSeatsAvailable, TestFixture)
{
    const std::vector<uint16_t> seats = service.getSeatsAvailable({"4DX"}, {"Fight Club"});
    BOOST_CHECK_EQUAL(seats.size(), 20);
}

BOOST_FIXTURE_TEST_CASE(TestGetSeatsAvailable_Premiere, TestFixture)
{
    const auto premiere = service.getPremiere({"4DX"}, {"Fight Club"});
    BOOST_CHECK_EQUAL(premiere.has_value(), true);

    const std::vector<uint16_t> seats = premiere.value()->getSeatsAvailable();
    BOOST_CHECK_EQUAL(seats.size(), 20);
}

BOOST_FIXTURE_TEST_CASE(TestBookSeatsAndGetSeatsAvailable, TestFixture)
{
    const auto premiere = service.getPremiere({"4DX"}, {"Fight Club"});
    BOOST_CHECK_EQUAL(premiere.has_value(), true);

    std::vector<uint16_t> seats = premiere.value()->getSeatsAvailable();
    BOOST_CHECK_EQUAL(seats.size(), 20);

    const bool ok = premiere.value()->bookSeats({1,2,3});
    BOOST_CHECK_EQUAL(ok, true);

    seats = premiere.value()->getSeatsAvailable();
    BOOST_CHECK_EQUAL(seats.size(), 17);
}

// ADD TEST SUITE NAME

// check premiere.value()->bookSeats({1,2,3})
//           OK - test1
//           FALSE - test2

// getPremiere test

//
/** Tests
	- Создать сервис
	- загрузить данные
	- View all playing movies    <------------------
	- View all theaters
	- See all theaters showing the movie
	-
	- Select a theater: ok
	- Select a theater: non exising
	- Select a theater: если выбран фильм короный там идет
	- Select a theater: если выбран фильм короный там  НЕ идет
	-
	-
	- Select a movie: ok
	- Select a movie: non exising
	- Select a movie: если выбран theaters в котором этот фильм идет
	- Select a movie: если выбран theaters в котором этот фильм НЕ идет
	-
	- список всех мест доступных cлотов
	- количество 20 макс
	- забукать одно - проверить список
	- забукать несколько - проверить список
	- забукать занятное - ошибка - проверить список
	- забукать seat 25
	- забукать seat 0
	- забукать seat -1
	- забукать Multithrading
    -
    - Movie class - равенство
    - theater class - равенство
    -
    -
**/


#if 0
class Calculator
{
public:
    explicit Calculator(int val) : value {val} {
    }

    void Divide(int val)
    {
        if (val == 0)
        {
            throw std::invalid_argument("Zero devistion");
        }
        value /= val;
    }

    void Multiply(int val)
    {
        value *= val;
    }

    [[nodiscard]]
    int Result() const
    {
        return value;
    }

private:
    int value {0};
};


struct Fixture
{
    Fixture(): calculator(12) {
        // Here the test object can be configured
    }

    // And here it is correct to finish working with it
    ~Fixture() = default;

    /** And here is the test object itself **/
    Calculator calculator;
};


BOOST_AUTO_TEST_CASE(MyTestCase)
{
    // To simplify this example test, let's suppose we'll test 'float'.
    // Some test are stupid, but all should pass.
    float x = 9.5f;

    BOOST_CHECK(x != 0.0f);
    BOOST_CHECK_EQUAL((int)x, 9);
    BOOST_CHECK_CLOSE(x, 9.5f, 0.0001f); // Checks differ no more then 0.0001%
}

BOOST_AUTO_TEST_CASE(testCalculator)
{
    Calculator calculator(12);
    BOOST_CHECK_EQUAL(calculator.Result(), 12);
}

BOOST_AUTO_TEST_CASE(testCalculatorDivide)
{
    Calculator calculator(12);
    calculator.Divide(3);
    BOOST_CHECK_EQUAL(calculator.Result(), 4);
    calculator.Divide(2);
    BOOST_CHECK_EQUAL(calculator.Result(), 2);
}

BOOST_AUTO_TEST_CASE(testCalculatorMultiply)
{
    Calculator calculator(12);
    calculator.Multiply(2);
    BOOST_CHECK_EQUAL(calculator.Result(), 24);
    calculator.Multiply(3);
    BOOST_CHECK_EQUAL(calculator.Result(), 72);
}

BOOST_FIXTURE_TEST_CASE(testCalculatorWithFixture, Fixture)
{
    BOOST_CHECK_EQUAL(calculator.Result(), 12);
}

BOOST_FIXTURE_TEST_CASE(testCalculatorDivideWithFixture, Fixture)
{
    calculator.Divide(3);
    BOOST_CHECK_EQUAL(calculator.Result(), 4);
    calculator.Divide(2);
    BOOST_CHECK_EQUAL(calculator.Result(), 2);
}

BOOST_FIXTURE_TEST_CASE(testCalculatorMultiplyWithFixture, Fixture)
{
    calculator.Multiply(2);
    BOOST_CHECK_EQUAL(calculator.Result(), 24);
    calculator.Multiply(3);
    BOOST_CHECK_EQUAL(calculator.Result(), 72);
}
#endif