/**============================================================================
Name        : BookingService.cpp
Created on  : 28.12.2023
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : BookingService.cpp
============================================================================**/

#include "BookingService.h"
#include <algorithm>
#include <iostream>

namespace Booking
{
    std::ostream& operator<<(std::ostream& stream, const Movie& movie) {
        stream << movie.name;
        return stream;
    }

    std::ostream& operator<<(std::ostream& stream, const Theater& theater) {
        stream << theater.name;
        return stream;
    }

    std::ostream& operator<<(std::ostream& stream, const Premiere& premiere) {
        stream << "Theatre: " << *premiere.theater << ", Movie: " << *premiere.movie << ". Seats available: ";
        for (int seatNum = 1; SeatStatus status: premiere.seats) {
            if (SeatStatus::Available == status)
                stream << seatNum << ' ';
            ++seatNum;
        }
        return stream;
    }

    std::ostream& operator<<(std::ostream& stream, const std::vector<uint16_t>& values) {
        for (uint16_t seatNum: values)
            stream << seatNum << " ";
        return stream;
    }
}

namespace Booking
{
    Premiere::Premiere(std::shared_ptr<Theater> theater, std::shared_ptr<Movie> movie):
            theater { std::move(theater) }, movie { std::move(movie) } {
    }

    std::vector<uint16_t> Premiere::getSeatsAvailable() const noexcept
    {
        std::vector<uint16_t> seatsAvailable;
        seatsAvailable.reserve(seats.size());

        for (int seatNum = 1; SeatStatus status: seats) {
            if (SeatStatus::Available == status)
                seatsAvailable.push_back(seatNum);
            ++seatNum;
        }

        seatsAvailable.shrink_to_fit();
        return seatsAvailable;
    }

    // TODO: Describe CAS
    bool Premiere::bookSeats(const std::vector<uint16_t>& seatsToBook)
    {
        std::lock_guard<std::mutex> lock {mtxBooking};
        std::array<SeatStatus, Theater::seatsCapacityMax> seatsCopy {seats};
        for (uint16_t seatNum: seatsToBook)
        {
            const uint16_t seatIdx = seatNum - 1;
            if (0 >= seatNum || seatNum > Theater::seatsCapacityMax || SeatStatus::Booked == seatsCopy[seatIdx])
                return false;
            seatsCopy[seatIdx] = SeatStatus::Booked;
        }

        seats.swap(seatsCopy);
        return true;
    }
}

namespace Booking
{
    // TODO: --> std::vector<std::string_view> ??
    std::vector<Movie> BookingService::getAllPlayingMovies() const
    {
        std::vector<Movie> allMovies;
        allMovies.reserve(movies.size());
        for (const MoviePtr& mov: movies)
            allMovies.push_back(*mov);
        return allMovies;
    }

    std::vector<Theater> BookingService::getTheaters() const
    {
        std::vector<Theater> allTheaters;
        allTheaters.reserve(movies.size());
        for (const TheaterPtr& theater: theaters)
            allTheaters.push_back(*theater);
        return allTheaters;
    }

    std::vector<Theater> BookingService::getTheatersByMovie(const Movie& movie) const
    {
        std::vector<Theater> allTheaters;
        for (const PremierePtr & premiere: bookingSchedule)
            if (premiere->movie->name == movie.name)
                allTheaters.push_back(*premiere->theater);
        return allTheaters;
    }

    // TODO: Refactor ?? ID's <-> HashTable
    std::optional<BookingService::PremierePtr>
    BookingService::getPremiere(const Theater& theater,
                                const Movie& movie) const
    {
        if (const auto premiereIter = std::find_if(bookingSchedule.cbegin(), bookingSchedule.cend(), [&](const auto& premiere) {
                return *(premiere->theater) == theater && *(premiere->movie) == movie;
            }); bookingSchedule.end() != premiereIter)
            return std::make_optional<PremierePtr>(*premiereIter);
        return std::nullopt;
    }

    std::vector<uint16_t> BookingService::getSeatsAvailable(const Theater& theater,
                                                            const Movie& movie) const
    {
        const std::optional<PremierePtr> premiere { getPremiere(theater, movie) };
        if (!premiere.has_value())
            return {};

        return premiere.value()->getSeatsAvailable();
    }

    void BookingService::addMovie(const std::string& movieName)
    {
        movies.push_back(std::make_shared<Movie>(movieName));
    }

    void BookingService::addTheater(const std::string& theaterName)
    {
        theaters.push_back(std::make_shared<Theater>(theaterName));
    }

    bool BookingService::scheduleMovie(const std::string& movieName,
                                       const std::string& theaterName)
    {
        const auto movie = std::find_if(movies.cbegin(), movies.cend(), [&](const auto& mov){
            return mov->name  == movieName;
        });
        if (movies.end() == movie)
            return false;

        const auto theater = std::find_if(theaters.cbegin(), theaters.cend(), [&](const auto& theater){
            return theater->name  == theaterName;
        });
        if (theaters.end() == theater)
            return false;

        bookingSchedule.emplace_back(std::make_shared<Premiere>(*theater, *movie));
        return true;
    }

    // Create some default data
    // TODO : Create some data provider : for tests ??
    void BookingService::initialize()
    {
        addMovie("Fight Club");
        addMovie("The Lord of the Rings: The Fellowship of the Ring");
        addMovie("The Lord of the Rings: The Two Towers");
        addMovie("The Lord of the Rings: The Return of the King");
        addMovie("The Green Mile");
        addMovie("The Shawshank Redemption");
        addMovie("Pulp Fiction");
        addMovie("Terminator");
        addMovie("Terminator 2: Judgment Day");
        addMovie("Inception");
        addMovie("Harry Potter and the Sorcerer's Stone");
        addMovie("Harry Potter and the Chamber of Secrets");
        addMovie("Harry Potter and the Goblet of Fire");
        addMovie("Harry Potter and the Prisoner of Azkaban");

        addTheater("Raj Mandir");
        addTheater("Alamo Drafthouse");
        addTheater("Cine Thisio");
        addTheater("Kino International");
        addTheater("4DX");
        addTheater("Uplink X");
        addTheater("Prasads");
        addTheater("Cine de Chef");
        addTheater("Castro Theatre");
        addTheater("Rooftop Cinema");
        addTheater("AMC Pacific Place Cinema");
        addTheater("Odeon");
        addTheater("Biograf Teater");
        addTheater("Electric Cinema");
        addTheater("Sun Pictures");

        scheduleMovie("Fight Club", "4DX") ;
        scheduleMovie("Fight Club", "Electric Cinema");
        scheduleMovie("The Green Mile", "4DX");
        scheduleMovie("Terminator", "4DX");
    }
}