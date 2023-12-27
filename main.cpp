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
#include <optional>
#include <thread>

#include "Poco/Net/DNS.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/StreamCopier.h"
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/ServerSocketImpl.h>
#include <Poco/Util/ServerApplication.h>


using Poco::Net::DNS;
using Poco::Net::IPAddress;
using Poco::Net::HostEntry;


namespace PocoService
{
    class Ping : public Poco::Net::HTTPRequestHandler {
    private:
        void handleRequest(Poco::Net::HTTPServerRequest &request,
                           Poco::Net::HTTPServerResponse &response) override {
            response.send().flush();
            response.setStatus(Poco::Net::HTTPServerResponse::HTTP_OK);
            std::cout << "Ping" << std::endl;
        }
    };


    class Factory : public Poco::Net::HTTPRequestHandlerFactory {
    private:
        Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &request) override {
            if (request.getMethod() != Poco::Net::HTTPRequest::HTTP_GET)
                return nullptr;

            if (request.getURI() == "/ping")
                return new Ping();

            return nullptr;
        }
    };

    class ServerSocket : public Poco::Net::ServerSocketImpl {
    public:
        using Poco::Net::SocketImpl::init;
    };

    class Socket : public Poco::Net::Socket {
    public:
        explicit Socket(const std::string &address) :
                Poco::Net::Socket(new ServerSocket()) {
            const Poco::Net::SocketAddress socket_address(address);
            auto *socket = dynamic_cast<ServerSocket *>(impl());
            socket->init(socket_address.af());
            socket->setReuseAddress(true);
            socket->setReusePort(false);
            socket->bind(socket_address, false);
            socket->listen();
        }
    };

    class Server: public Poco::Util::ServerApplication
    {
        int main(const std::vector<std::string>& args) override
        {
            Poco::Net::HTTPServerParams::Ptr parameters = new Poco::Net::HTTPServerParams();
            parameters->setTimeout(10000);
            parameters->setMaxQueued(100);
            parameters->setMaxThreads(4);

            const Poco::Net::ServerSocket socket(Socket("0.0.0.0:8080"));

            Poco::Net::HTTPServer server(new Factory(), socket, parameters);

            server.start();
            waitForTerminationRequest();
            server.stopAll();
        }
    };


    void runServer()
    {
        Poco::Net::ServerSocket srv(8080); // does bind + listen
        while (true)
        {
            Poco::Net::StreamSocket ss = srv.acceptConnection();
            Poco::Net::SocketStream str(ss);
            str << "HTTP/1.0 200 OK\r\n"
                   "Content-Type: text/html\r\n"
                   "\r\n"
                   "<html><head><title>My 1st Web Server</title></head>"
                   "<body><h1>Hello, world!!!</h1></body></html>"
                << std::flush;
        }
    }

}


namespace Service
{
    /*
    struct Seat
    {
        uint16_t id {0};
        bool booked {false};
    };
    */

    // TODO: Add operator<< | Serialization
    struct Movie
    {
        std::string name;

        std::strong_ordering operator<=>(const Movie&) const = default;
    };

    struct Theater
    {
        static constexpr uint16_t seatsCapacityMax { 20 };
        std::string name;

        std::strong_ordering operator<=>(const Theater&) const = default;
    };

    enum class SeatStatus {
        Available,
        Booked
    };

    struct Premiere
    {
        std::shared_ptr<Theater> theater;
        std::shared_ptr<Movie> movie;
        std::array<SeatStatus, Theater::seatsCapacityMax> seats {};

        //mutable std::mutex mtxBooking;

        Premiere(std::shared_ptr<Theater> theater, std::shared_ptr<Movie> movie):
                theater { std::move(theater) }, movie { std::move(movie) } {
        }

        [[nodiscard]]
        std::vector<uint16_t> getSeatsAvailable() const noexcept
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

        // CAS
        bool bookSeats(const std::vector<uint16_t>& seatsToBook)
        {
            //std::lock_guard<std::mutex> lock {mtxBooking};

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

        // friend std::ostream& operator<<(std::ostream& stream, const Premiere& premiere);
    };


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


    struct BookingService
    {
        using MoviePtr = std::shared_ptr<Movie>;
        using TheaterPtr = std::shared_ptr<Theater>;

        std::vector<MoviePtr> movies;
        std::vector<TheaterPtr> theaters;
        std::vector<Premiere> bookingSchedule;

        // TODO: --> std::vector<std::string_view> ??
        [[nodiscard]]
        std::vector<Movie> getAllPlayingMovies() const
        {
            std::vector<Movie> allMovies;
            allMovies.reserve(movies.size());
            for (const MoviePtr& mov: movies)
                allMovies.push_back(*mov);
            return allMovies;
        }

        [[nodiscard]]
        std::vector<Theater> getTheaters() const
        {
            std::vector<Theater> allTheaters;
            allTheaters.reserve(movies.size());
            for (const TheaterPtr& mov: theaters)
                allTheaters.push_back(*mov);
            return allTheaters;
        }

        [[nodiscard]]
        std::optional<Premiere> getPremiere(const Theater& theater,
                                            const Movie& movie) const
        {
            if (const auto premiereIter = std::find_if(bookingSchedule.cbegin(), bookingSchedule.cend(), [&](const auto& premiere) {
                return *(premiere.theater) == theater && *(premiere.movie) == movie;
            }); bookingSchedule.end() != premiereIter)
                return std::make_optional<Premiere>(*premiereIter);
            return std::nullopt;
        }

        [[nodiscard]]
        std::vector<uint16_t> getSeatsAvailable(const Theater& theater,
                                                const Movie& movie) const
        {
            const std::optional<Premiere> premiere { getPremiere(theater, movie) };
            if (!premiere.has_value())
                return {};

            return premiere->getSeatsAvailable();
        }

        [[nodiscard]]
        std::vector<Theater> getTheaters(const Movie& movie) const
        {
            return {};
        }

        // Create some default data
        void initialize()
        {
            movies.push_back(std::make_shared<Movie>("Fight Club"));
            movies.push_back(std::make_shared<Movie>("The Lord of the Rings: The Fellowship of the Ring"));
            movies.push_back(std::make_shared<Movie>("The Lord of the Rings: The Two Towers"));
            movies.push_back(std::make_shared<Movie>("The Lord of the Rings: The Return of the King"));
            movies.push_back(std::make_shared<Movie>("The Green Mile"));
            movies.push_back(std::make_shared<Movie>("The Shawshank Redemption"));
            movies.push_back(std::make_shared<Movie>("Pulp Fiction"));
            movies.push_back(std::make_shared<Movie>("Terminator"));
            movies.push_back(std::make_shared<Movie>("Terminator 2: Judgment Day"));
            movies.push_back(std::make_shared<Movie>("Inception"));
            movies.push_back(std::make_shared<Movie>("Harry Potter and the Sorcerer's Stone"));
            movies.push_back(std::make_shared<Movie>("Harry Potter and the Chamber of Secrets"));
            movies.push_back(std::make_shared<Movie>("Harry Potter and the Goblet of Fire"));
            movies.push_back(std::make_shared<Movie>("Harry Potter and the Prisoner of Azkaban"));

            theaters.push_back(std::make_shared<Theater>("Raj Mandir"));
            theaters.push_back(std::make_shared<Theater>("Alamo Drafthouse"));
            theaters.push_back(std::make_shared<Theater>("Cine Thisio"));
            theaters.push_back(std::make_shared<Theater>("Kino International"));
            theaters.push_back(std::make_shared<Theater>("4DX"));
            theaters.push_back(std::make_shared<Theater>("Uplink X"));
            theaters.push_back(std::make_shared<Theater>("Prasads"));
            theaters.push_back(std::make_shared<Theater>("Cine de Chef"));
            theaters.push_back(std::make_shared<Theater>("Castro Theatre"));
            theaters.push_back(std::make_shared<Theater>("Rooftop Cinema"));
            theaters.push_back(std::make_shared<Theater>("AMC Pacific Place Cinema"));
            theaters.push_back(std::make_shared<Theater>("Odeon"));
            theaters.push_back(std::make_shared<Theater>("Biograf Teater"));
            theaters.push_back(std::make_shared<Theater>("Electric Cinema"));
            theaters.push_back(std::make_shared<Theater>("Sun Pictures"));

            /* Create scheduleMove function ?*/
            bookingSchedule.emplace_back(theaters[4] /* 4DX */, movies[0] /* Fight Club*/);
            bookingSchedule.emplace_back(theaters[4] /* 4DX */, movies[4] /* The Green Mile*/);
        }
    };
}


namespace Tests
{
    using namespace Service;

    Service::BookingService service;

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

        std::vector<uint16_t> seats = service.getSeatsAvailable({"4DX"}, {"Fight Club"});
        std::cout <<seats << std::endl;
    }

    void getSeatsAvailableAfterBookingTest()
    {
        service.initialize();
        std::optional<Premiere> premiere = service.getPremiere({"4DX"}, {"Fight Club"});
        if (!premiere.has_value())
            return;

        std::cout << premiere->getSeatsAvailable() << std::endl;
        std::cout << std::boolalpha << premiere->bookSeats({1,2,3}) << std::endl;
        std::cout << premiere->getSeatsAvailable() << std::endl;
        std::cout << std::boolalpha << premiere->bookSeats({1,2,3}) << std::endl;
        std::cout << premiere->getSeatsAvailable() << std::endl;
        std::cout << std::boolalpha << premiere->bookSeats({4, 5, 20}) << std::endl;
        std::cout << premiere->getSeatsAvailable() << std::endl;
    }

    void getAllScheduleTest()
    {
        service.initialize();
        for (const Premiere& premiere: service.bookingSchedule)
            std::cout << premiere << std::endl;
    }
}



int main([[maybe_unused]] int argc,
         [[maybe_unused]] char** argv)
{
    using namespace Service;
    const std::vector<std::string_view> args(argv + 1, argv + argc);


    /*
    Networking::Server::runServer();

    PocoService::Server server;
    return server.run(argc, argv);
    */


    // Tests::getAllMoviesTest();
    // Tests::getAllTheatersTest();
    // Tests::getSeatsAvailableTest();
    Tests::getSeatsAvailableAfterBookingTest();
    // Tests::getAllScheduleTest();

    return EXIT_SUCCESS;
}
