/**============================================================================
Name        : CLI.h
Created on  : 28.12.2023
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : CLI.h
============================================================================**/

#ifndef BOOKINGSERVICE_CLI_H
#define BOOKINGSERVICE_CLI_H

#include "BookingService.h"
#include <iostream>
#include <functional>
#include <unordered_map>

namespace CLI
{
    using namespace std::string_view_literals;

    struct Handler
    {
        Booking::BookingService& service;
        std::basic_ostream<char>& outStream;

        std::optional<Booking::Theater> theaterSelected { std::nullopt };
        std::optional<Booking::Movie> movieSelected { std::nullopt };

        Handler(Booking::BookingService& service,
                std::basic_ostream<char>& out = std::cout);

        [[nodiscard]]
        bool debug(std::string_view name);

        [[nodiscard]]
        bool findTheaters(std::string_view name);

        [[nodiscard]]
        bool selectTheater(std::string_view name);

        [[nodiscard]]
        bool selectMovie(std::string_view name);

        [[nodiscard]]
        bool listAvailableSeats(std::string_view name);

        [[nodiscard]]
        bool printTheaters(std::string_view);

        [[nodiscard]]
        bool printMovies(std::string_view);
    };


    class SimpleCLI
    {
        using HandlerType = Handler;
        using methodPtr_t = bool (HandlerType::*)(std::string_view params);

        enum class Status {
            Ok,
            Continue,
            Stop
        };

        template<typename... Args>
        bool calFunction(methodPtr_t func, Args&&... params)
        {
            return std::invoke(func , handlerPtr, std::forward<Args>(params)...);
        }

        [[nodiscard]]
        static std::vector<std::string_view> splitCommand(std::string_view input);

        [[nodiscard]]
        static std::pair<std::string_view, std::string_view> extractCommand(std::string_view input);

        [[nodiscard]]
        Status validateCommand(std::string_view command) const;

    public:
        explicit SimpleCLI(HandlerType *ptr, std::basic_ostream<char>& out = std::cout);
        void start();

        [[nodiscard]]
        Status processCommand(std::string_view userInput);

    private:

        static inline const std::unordered_map<std::string_view, methodPtr_t> funcMapping
        {
            {"debug"sv, &HandlerType::debug},
            {"find_theaters"sv, &HandlerType::findTheaters},
            {"select_theater"sv, &HandlerType::selectTheater},
            {"select_movie"sv, &HandlerType::selectMovie},
            {"list_available_seats"sv, &HandlerType::listAvailableSeats},
            {"list_theaters"sv, &HandlerType::printTheaters},
            {"list_movies"sv, &HandlerType::printMovies},
        };

        HandlerType *handlerPtr;
        std::basic_ostream<char>& outStream;
    };

};

#endif //BOOKINGSERVICE_CLI_H
