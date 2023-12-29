/**============================================================================
Name        : BookingService.h
Created on  : 28.12.2023
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : BookingService.h
============================================================================**/

#ifndef BOOKINGSERVICE_BOOKINGSERVICE_H
#define BOOKINGSERVICE_BOOKINGSERVICE_H

#include <string>
#include <vector>
#include <array>
#include <memory>
#include <optional>

namespace Booking
{
    enum class SeatStatus: bool {
        Available,
        Booked
    };

    // TODO: Static unique ID
    struct Movie
    {
        std::string name;

        std::strong_ordering operator<=>(const Movie&) const = default;
    };

    // TODO: Static unique ID
    struct Theater
    {
        static constexpr uint16_t seatsCapacityMax { 20 };
        std::string name;

        std::strong_ordering operator<=>(const Theater&) const = default;
    };

    struct Premiere
    {
        // TODO: to ID's
        std::shared_ptr<Theater> theater;
        std::shared_ptr<Movie> movie;
        std::array<SeatStatus, Theater::seatsCapacityMax> seats {};

        mutable std::mutex mtxBooking;

        Premiere(std::shared_ptr<Theater> theater, std::shared_ptr<Movie> movie);

        [[nodiscard]]
        std::vector<uint16_t> getSeatsAvailable() const noexcept;

        bool bookSeats(const std::vector<uint16_t>& seatsToBook);
    };

    // TODO: DOCUMENTATION
    struct BookingService
    {
        using MoviePtr = std::shared_ptr<Movie>;
        using TheaterPtr = std::shared_ptr<Theater>;
        using PremierePtr = std::shared_ptr<Premiere>;

        std::vector<MoviePtr> movies;
        std::vector<TheaterPtr> theaters;
        std::vector<PremierePtr> bookingSchedule;

        [[nodiscard]]
        std::vector<Movie> getAllPlayingMovies() const;

        [[nodiscard]]
        std::vector<Theater> getTheaters() const;

        [[nodiscard]]
        std::vector<Theater> getTheatersByMovie(const Movie& movie) const;

        [[nodiscard]]
        std::optional<PremierePtr> getPremiere(const Theater& theater,
                                               const Movie& movie) const;
        [[nodiscard]]
        std::vector<uint16_t> getSeatsAvailable(const Theater& theater,
                                                const Movie& movie) const;

        // Test method
        void addMovie(const std::string& movieName);
        void addTheater(const std::string& theaterName);
        bool scheduleMovie(const std::string& movieName, const std::string& theaterName);

        // Create some default data
        void initialize();
    };


    std::ostream& operator<<(std::ostream& stream, const Movie& movie);
    std::ostream& operator<<(std::ostream& stream, const Theater& theater);
    std::ostream& operator<<(std::ostream& stream, const Premiere& premiere);
    std::ostream& operator<<(std::ostream& stream, const std::vector<uint16_t>& values);
};

#endif //BOOKINGSERVICE_BOOKINGSERVICE_H
