/**
 * @file       BookingService.h
 * @date       29.12.2023
 * @author     Andrei Tokmakov
 * @version    1.0
 * @copyright  Your copyright notice
 * @brief      BookingService modules definition
*/

#ifndef BOOKINGSERVICE_BOOKINGSERVICE_H
#define BOOKINGSERVICE_BOOKINGSERVICE_H

#include <string>
#include <vector>
#include <array>

#include "Database.h"

namespace Booking
{
    using namespace DB;

    enum class SeatStatus: bool {
        Available,
        Booked
    };

    struct Movie: TableEntry<Movie>
    {
        explicit Movie(std::string name): TableEntry {std::move(name)} {
        }
    };

    struct Theater: TableEntry<Theater>
    {
        static constexpr uint16_t seatsCapacityMax { 20 };

        explicit Theater(std::string name): TableEntry {std::move(name)} {
        }
    };

    // TODO: Add more info
    /**
     * @brief Premiere class
     */
    struct Premiere
    {
        size_t theaterId {0};
        size_t movieId {0};
        std::array<SeatStatus, Theater::seatsCapacityMax> seats {};

        mutable std::mutex mtxBooking;

        /**
         * Create a new Premiere class object instance
         * @brief Constructor.
         * @param theater a reference to the Theater class object instance.
         * @param movie a reference to the Movie class object instance.
         * @see Premiere(const Theater& theater, const Movie& movie)
        */
        Premiere(const Theater& theater, const Movie& movie);

        /**
         * @brief Returns a list of theater seats available for booking
         * @return Object of the std::vector class with the numbers of seats.
         */
        [[nodiscard("Please check the result: Expensive to call")]]
        std::vector<uint16_t> getSeatsAvailable() const noexcept;

        /**
         * @brief Performs a booking/ reservations for the specified seats
         * @return True - in case of successful booking of the specified seats
         *         False - otherwise
         */
        [[nodiscard("Please check the result: Expensive to call")]]
        bool bookSeats(const std::vector<uint16_t>& seatsToBook);
    };

    struct BookingService
    {
        using PremierePtr = std::shared_ptr<Premiere>;

        Table<Movie> movies;
        Table<Theater> theaters;
        std::vector<PremierePtr> bookingSchedule;

        /**
         * Tries to find a Movie type object in the database by name
         * @param name The name of the movie
         * @return an object of type std::optional containing a pointer to the Movie,
         *          if it could not be found, then std::null
        */
        [[nodiscard]]
        std::optional<Movie*> findMovie(const std::string& name) const;

        /**
         * Tries to find a Theater type object in the database by name
         * @param name The name of the theater
         * @return an object of type std::optional containing a pointer to the Theater,
         *          if it could not be found, then std::nullopt
        */
        [[nodiscard]]
        std::optional<Theater*> findTheater(const std::string& name) const;

        // TODO: do not participate in the Movie* lifycycle
        /**
         * Returns a collection of all available (existing) movies
         * @return a std::vector collection Movie* objects
        */
        [[nodiscard]]
        std::vector<Movie*> getMovies() const;

        // TODO: do not participate in the Movie* lifycycle
        /**
         * Returns a collection of all available (existing) theaters
         * @return a std::vector collection Movie* objects
        */
        [[nodiscard]]
        std::vector<Theater*> getTheaters() const;

        /**
         * Returns a list of movies currently playing in theaters
         * @return a std::vector collection Movie* objects.
         * If there are no such films at the moment, an empty vector will be returned
        */
        [[nodiscard]]
        std::vector<Movie*> getPlayingMovies() const;

        /**
         * Returns all theaters showing the movie
         * @param movieName The name of the movie
         * @return a std::vector collection Movie* objects.
        */
        [[nodiscard]]
        std::vector<Theater*> getTheatersByMovie(const std::string& movieName) const;

        /**
         * Returns the premiere (an object of the Premiere class) of the specified movie in the specified cinema, if any
         * @param theater Theater class instance pointer (the Theater where the film is being shown)
         * @param movie Movie class instance pointer (Movie which is expected to be shown at the specified Theater)
         * @return an object of type std::optional containing a pointer to the Premiere,
         *          if it could not be found, then std::null
        */
        [[nodiscard]]
        std::optional<PremierePtr> getPremiere(const Theater* const theater,
                                               const Movie* const movie) const;

        /**
         * Returns the premiere (an object of the Premiere class) of the specified movie in the specified cinema, if any
         * @param theaterName the Theater name
         * @param movieName the Movie name
         * @return an object of type std::optional containing a pointer to the Premiere,
         *          if it could not be found, then std::null
         *
         * @note The method is less productive than
         * BookingService::getPremiere(const Theater* const, const Movie* const) const \n
         * since it additionally searches for the appropriate Movie and Theater in the database by
         * their names and only after \n that, in case of successful search, start trying to find corresponding Premiere
         * \n\n An overloaded version of this function should be preferred if possible
        */
        [[nodiscard]]
        std::optional<PremierePtr> getPremiere(const std::string& theaterName,
                                               const std::string & movieName) const;

        /**
         * Returns the premiere (an object of the Premiere class) of the specified movie in the specified cinema, if any
         * @param theater Theater class instance pointer (the Theater where the film is being shown)
         * @param movie Movie class instance pointer (Movie which is expected to be shown at the specified Theater)
         * @return an object of type std::optional containing a pointer to the Premiere,
         *          if it could not be found, then std::null
        */
        [[nodiscard]]
        std::vector<uint16_t> getSeatsAvailable(const Theater* const theater,
                                                const Movie* const movie) const;
        [[nodiscard]]
        std::vector<uint16_t> getSeatsAvailable(const std::string& theaterName,
                                                const std::string& movieName) const;

        // Test method
        void addMovie(const std::string& movieName);
        void addTheater(const std::string& theaterName);
        bool scheduleMovie(const std::string& movieName, const std::string& theaterName);

        // Create some default data
        void initialize();
    };
};

#endif //BOOKINGSERVICE_BOOKINGSERVICE_H
