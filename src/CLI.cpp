/**============================================================================
Name        : CLI.cpp
Created on  : 28.12.2023
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : CLI.cpp
============================================================================**/

#include "CLI.h"
#include <iostream>
#include <iomanip>
#include <charconv>

namespace CLI
{
    using namespace std::string_view_literals;
    using namespace Booking;

    SimpleCLI::SimpleCLI(BookingService &service, std::basic_ostream<char> &out) :
            service{service }, outStream {out} {
    }

    bool SimpleCLI::book_seats(std::string_view params)
    {
        if (params.empty()) {
            outStream << "Seats numbers expected\n";
            return true;
        } else if (!movieSelected || !theaterSelected) {
            outStream << "No theater or Movie selected\n";
            return true;
        }

        std::vector<uint16_t> seatsToBook;
        auto extractSeats = [&seatsToBook](const std::vector<std::string_view>& parts){
            for (const std::string_view& seatNum: parts) {
                const auto [_, error] =
                        std::from_chars(seatNum.data(), seatNum.data() + seatNum.size(), seatsToBook.emplace_back());
                if (error != std::errc{})
                    return false;
            }
            return true;
        };

        bool parsed = false;
        if (std::string_view::npos != params.find(','))
            parsed = extractSeats(split(params, ","));
        else
            parsed = extractSeats({params});

        if (!parsed) {
            outStream << "Incorrect syntax: '" << params << "'. Format expected: [1,2,3,4,5] or [1]";
            return true;
        }

        const auto premiere = service.getPremiere(theaterSelected.value(), movieSelected.value());
        if (!premiere.value()->bookSeats(seatsToBook)) {
            outStream << "Sorry: Failed to book seats: " << params << std::endl;
        } else {
            outStream << "Seats " << params  << " are booked\n";
        }

        return true;
    }

    bool SimpleCLI::listAvailableSeats(std::string_view)
    {
        if (!movieSelected) {
            outStream << "Please select a Movie to see the available slots\n";
        }
        else if (!theaterSelected) {
            outStream << "Please select a Theater to see the available slots\n";
        }
        else {
            const std::vector<uint16_t> seats = service.getSeatsAvailable(theaterSelected.value() ,movieSelected.value() );
            outStream << "Theater: " << theaterSelected.value() << ", Movie : " << movieSelected.value()<< std::endl;
            outStream << "Seats available: " << seats << std::endl;
        }
        return true;
    }

    bool SimpleCLI::findTheaters(std::string_view name)
    {
        if (name.empty()) {
            outStream << "Movie name expected\n";
            return false;
        }
        const Movie movie {name.data()};
        const std::vector<Theater> allTheaters = service.getTheatersByMovie(movie);

        outStream << "The movie '" << movie << "' is being shown in:\n";
        for (const auto& theater: allTheaters)
            outStream << '\t' << theater.name << std::endl;

        return true;
    }

    bool SimpleCLI::isPremiereExist(const Booking::Theater& theater,
                                    const Booking::Movie& movie) const noexcept
    {
        if (!service.getPremiere(theater,movie).has_value()) {
            outStream << "Unfortunately, the " << movie << " movie is not being shown at the " << theater << " cinema\n";
            return false;
        }
        return true;
    }

    bool SimpleCLI::selectTheater(std::string_view name)
    {
        theaterSelected.reset();
        if (name.empty()) {
            outStream << "Theater name expected\n";
            return false;
        } else if (movieSelected && !isPremiereExist( {name.data() },movieSelected.value())) {
            return true;
        }

        const std::vector<Theater> allTheaters = service.getTheaters();
        if (const auto iter = std::find_if(allTheaters.cbegin(), allTheaters.cend(), [&](const Theater& theater) {
                return theater.name == name;
            }); allTheaters.end() != iter) {
            theaterSelected = *iter;
            outStream << "The " << theaterSelected.value() << " theater is chosen\n";
        } else {
            outStream << "Failed to find the '" << name << "' theater\n";
        }
        return true;
    }

    bool SimpleCLI::selectMovie(std::string_view name)
    {
        movieSelected.reset();
        if (name.empty()) {
            outStream << "Movie name expected\n";
            return false;
        } else if (theaterSelected && !isPremiereExist(theaterSelected.value(),  {name.data() })) {
            return true;
        }

        const std::vector<Booking::Movie> movies = service.getAllPlayingMovies();
        if (const auto iter = std::find_if(movies.cbegin(), movies.cend(), [&](const Booking::Movie& mov) {
                return mov.name == name;
            }); movies.end() != iter) {
            movieSelected = *iter;
            outStream << "The " << movieSelected.value() << " movie is chosen\n";
        } else {
            outStream << "Failed to find the '" << name << "' movie\n";
        }
        return true;
    }

    bool SimpleCLI::printTheaters(std::string_view)
    {
        const std::vector<Booking::Theater> allTheaters = service.getTheaters();
        for (const auto& theater: allTheaters)
            outStream << '\t' << theater.name << std::endl;

        return true;
    }

    bool SimpleCLI::printMovies(std::string_view)
    {
        const std::vector<Booking::Movie> allMovies = service.getAllPlayingMovies();
        for (const auto& movie: allMovies)
            outStream << '\t' << movie.name << std::endl;

        return true;
    }

    std::vector<std::string_view> SimpleCLI::split(std::string_view input,
                                                   std::string_view delim)
    {
        std::vector<std::string_view> output;
        for (size_t first = 0, size = input.size(), second = 0; first < size; ) {
            second = input.find(delim, first);
            if (first != second)
                output.emplace_back(input.substr(first, second - first));
            if (second == std::string_view::npos)
                break;
            first = second + 1;
        }
        return output;
    }

    std::pair<std::string_view, std::string_view> SimpleCLI::extractCommand(std::string_view input)
    {
        const size_t spaceIdx = input.find(' ');
        if (std::string_view::npos == spaceIdx)
            return std::make_pair(input, "");

        return std::make_pair(input.substr(0, spaceIdx),
                              input.substr(spaceIdx + 1, input.size() - spaceIdx - 1));
    }

    SimpleCLI::Status SimpleCLI::validateCommand(std::string_view command) const
    {
        if (command.empty()) {
            outStream << "Incorrect input: Empty command" << std::endl;
            return Status::Continue;
        }

        if (command == "quit"sv || command == "q"sv) {
            return Status::Stop;
        } else if (command == "help"sv) {
            outStream << "Command available:\n";
            for (const auto& [cmd, F]: funcMapping)
                outStream << '\t' << cmd << std::endl;
            outStream << std::endl;
        }

        return Status::Ok;
    }

    SimpleCLI::Status SimpleCLI::processCommand(std::string_view userInput)
    {
        const auto& [cmd, params] = extractCommand(userInput);
        const Status status = validateCommand(cmd);
        if (Status::Continue == status || Status::Stop == status)
            return status;

        if (const auto funcIter = funcMapping.find(cmd); funcMapping.end() != funcIter) {
            if (calFunction(funcIter->second, params))
                return Status::Continue;
        }

        outStream << "Invalid command '" << cmd << "'\n";
        return Status::Continue;
    }

    void SimpleCLI::start()
    {
        Status status = Status::Ok;
        std::string line;
        while (Status::Stop != status) {
            outStream << "> ";
            std::getline(std::cin, line);
            status = processCommand(line);
        }
    }
}