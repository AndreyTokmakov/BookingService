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

//! Contains base parts and features of the Booking Service implementation
namespace Booking
{
    using namespace DB;

    /** SeatStatus.
     *  To enable type-safe seat booking handling (instead of bool)
     */
    enum class SeatStatus: bool {
        Available,
        Booked
    };

    /**
     * @brief Represents a Movie DataBase entry/record
     */
    struct Movie: TableEntry<Movie>
    {
        explicit Movie(std::string name): TableEntry (std::move(name)) {
        }
    };

    /**
     * @brief Represents a Theater DataBase entry/record
     */
    struct Theater: TableEntry<Theater>
    {
        /** Maximum number of seats the each Theater have **/
        static constexpr uint16_t seatsCapacityMax { 20 };

        explicit Theater(std::string name): TableEntry {std::move(name)} {
        }
    };

    /**
     * @brief Premiere class: To combine the relationship of Theater, Movie and the status of seats for the audience
     */
    struct Premiere
    {
        /** The unique identifier (from the database) of the Theater where the Movie with
         *  the appropriate movieId will be shown */
        size_t theaterId {0};

        /** The unique identifier (from the database) of the Movie that will
         *  be shown in the Theater with the appropriate theaterId */
        size_t movieId {0};

        /** Maximum number of seats the each Theater have **/
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

    /**
     * @brief BookingService class
     * Has access to a Database of Theater's and Movie's<br>
     * Database - its just a simple DB-like In Memory indexed tables, to enable fast O(n) access to
     * corresponding TableEntries by Name (std::string) or ID (size_t)
     */
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
         * @note The method is less performant than
         * BookingService::getPremiere(const Theater* const, const Movie* const) const \n
         * since it additionally searches for the appropriate Movie and Theater in the database by
         * their names and only after \n that, in case of successful search, start trying to find corresponding Premiere
         * \n\n An overloaded version of this function should be preferred if possible
        */
        [[nodiscard]]
        std::optional<PremierePtr> getPremiere(const std::string& theaterName,
                                               const std::string & movieName) const;

        /**
         * Returns a list of theater seats available for booking
         * @param theater Theater class instance pointer
         * @param movie Movie class instance pointer
         * @return Object of the std::vector class with the numbers of seats. In case if all seats already booked a empty
         * collection shall be return
         * @throw nothrow
        */
        [[nodiscard]]
        std::vector<uint16_t> getSeatsAvailable(const Theater* const theater,
                                                const Movie* const movie) const;

        /**
         * Returns a list of theater seats available for booking
         * @param theaterName the Theater name
         * @param movieName the Movie name
         * @return Object of the std::vector class with the numbers of seats. In case if all seats already booked a empty
         * collection shall be return
         *
         * @note The method is less performant than
         * BookingService::getSeatsAvailable(const Theater* const, const Movie* const) const
        */
        [[nodiscard]]
        std::vector<uint16_t> getSeatsAvailable(const std::string& theaterName,
                                                const std::string& movieName) const;

        /**
         * Adds a Movie to the Database by its name
         * @param movieName the Movie name
         * @note A test function.
        */
        void addMovie(const std::string& movieName);

        /**
         * Adds a Theater to the Database by its name
         * @param theaterName the Theater name
         * @note A test function.
        */
        void addTheater(const std::string& theaterName);

        /**
         * Schedule a premiere (Premiere) for the specific Movie ad the specific Theater
         * @param movieName the Movie name
         * @param theaterName the Theater name
         * @note A test function.
        */
        bool scheduleMovie(const std::string& movieName, const std::string& theaterName);

        /**
         * It is used for the purpose of initializing the test dataset
         * @note A test function.
        */
        void initialize();
    };
};

#endif //BOOKINGSERVICE_BOOKINGSERVICE_H
