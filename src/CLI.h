/**
 * @file       CLI.h
 * @date       29.12.2023
 * @author     Andrei Tokmakov
 * @version    1.0
 * @copyright  Your copyright notice
 * @brief      CLI modules declaration
*/

#ifndef BOOKINGSERVICE_CLI_H
#define BOOKINGSERVICE_CLI_H

#include "BookingService.h"
#include <iostream>
#include <functional>
#include <unordered_map>

//! Command Line Interface for the BookingService
namespace CLI
{
    using namespace std::string_view_literals;

    /**
     * A class that implements and emulates the functionality of a simple command line interface
    */
    class SimpleCLI
    {
    public:

        /**
         * Constructor: Create a new SimpleCLI class object instance
         * @param service a reference to the existing Booking::BookingService instance
         * @param out output stream (Default value: std::cout)
        */
        SimpleCLI(Booking::BookingService &service,
                  std::basic_ostream<char> &out = std::cout);

        /**  Status.
         *  A set of values/statuses for managing the command processing cycle from the user
         */
        enum class Status {
            Ok,
            Continue, /**< Continue processing */
            Stop      /**< Stop processing */
        };

        /**
         * Function is called to process each specific user input
         * @param userInput user input (from std::cin)
        */
        [[nodiscard]]
        Status processCommand(std::string_view userInput);

        /**
         * Starts the CLI
         * @return No-return
        */
        void start();

    private:

        using CmdHandlerType = SimpleCLI;
        using methodPtr_t = bool (CmdHandlerType::*)(std::string_view params);

        template<typename... Args>
        bool calFunction(methodPtr_t func, Args&&... params)
        {
            return std::invoke(func , this, std::forward<Args>(params)...);
        }

        /**
         * Method to process the <b>book_seats</b> command.
         * @param name user input (from std::cin) : Single of seats number of list of seats
        */
        [[nodiscard]]
        bool book_seats(std::string_view name);

        /**
         * Method to process the <b>find_theaters</b> command.
         * @param name user input (from std::cin) : the Theater name
        */
        [[nodiscard]]
        bool findTheaters(std::string_view name);

        /**
          * Method to process the <b>list_playing_movies</b> command.
          * @param None no input parameters required
         */
        [[nodiscard]]
        bool listPlayingMovies(std::string_view);

        /**
          * Method to process the <b>select_theater</b> command.
          * @param name user input - the Theater name
          * @note Chooses a Theater by its name:
          *  - Searches for the theater in the Database by its name
          *  - If the Theater is <i>not found</i>, the function exits
          *  - If the Theater is <i>found</i>, and the movie has already been pre-selected, the function checks whether
          *  corresponding premiere existing (if pre-selected movie is being shown in the specified Cinema)
          *      -# If not, the the function exits
          *      -# If corresponding premiere <i>existing</i> - function save the selected Theater
          */
        [[nodiscard]]
        bool selectTheater(std::string_view name);

        /**
          * Method to process the <b>select_movie</b> command.
          * @param name user input - the Movie name
          * @note Chooses a Movie by its name:
          *  - Searches for the movie in the Database by its name
          *  - If the Movie is <i>not found</i>, the function exits
          *  - If the Movie is <i>found</i>, and the Theater has already been pre-selected, the function checks whether
          *  corresponding premiere existing (if that movie is being shown in the pre-selected Theater)
          *      -# If not, the the function exits
          *      -# If corresponding premiere <i>existing</i> - function save the selected Movie
          */
        [[nodiscard]]
        bool selectMovie(std::string_view name);

        /**
          * Method to process the <b>list_available_seats</b> command.
          * @param None no input parameters required
         */
        [[nodiscard]]
        bool listAvailableSeats(std::string_view);

        /**
          * Method to process the <b>list_theaters</b> command.
          * @param None no input parameters required
         */
        [[nodiscard]]
        bool listAllTheaters(std::string_view);

        /**
          * Method to process the <b>list_movies</b> command.
          * @param None no input parameters required
         */
        [[nodiscard]]
        bool listAllMovies(std::string_view);

        [[nodiscard]]
        static std::vector<std::string_view> split(std::string_view input,
                                                   std::string_view delim = " "sv);

        [[nodiscard]]
        static std::pair<std::string_view, std::string_view> extractCommand(std::string_view input);

        [[nodiscard]]
        Status validateCommand(std::string_view command) const;

    private:

        static inline const std::unordered_map<std::string_view, methodPtr_t> funcMapping
        {
            {"book_seats"sv, &CmdHandlerType::book_seats},
            {"find_theaters"sv, &CmdHandlerType::findTheaters},
            {"list_playing_movies"sv, &CmdHandlerType::listPlayingMovies},
            {"select_theater"sv, &CmdHandlerType::selectTheater},
            {"select_movie"sv, &CmdHandlerType::selectMovie},
            {"list_available_seats"sv, &CmdHandlerType::listAvailableSeats},
            {"list_theaters"sv, &CmdHandlerType::listAllTheaters},
            {"list_movies"sv, &CmdHandlerType::listAllMovies},
       };

        Booking::BookingService& service;
        std::basic_ostream<char>& outStream;

        std::optional<Booking::Theater*> theaterSelected { std::nullopt };
        std::optional<Booking::Movie*> movieSelected { std::nullopt };
    };
};

#endif //BOOKINGSERVICE_CLI_H