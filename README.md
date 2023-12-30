# BookingService
Demo BookingService 

0. [Overview](#Overview)
1. [Download](#Download)
2. [Build](#Build)
3. [Requirements](#Requirements)
4. [Tests ](#Tests)

<a name="Overview"></a>
## Overview
Is C++ implementation of the simple movie booking service: to book seats for the selected movie at the selected theater
Service should be able to:
- View all playing movies
- See all theaters showing the movie
- See available seats (e.g., a1, a2, a3) for the selected theater & movie.
    You can assume that all theaters have 20 seats capacity
- Select a theater
- Select a movie
- Book one or more of the available seats. Service should be able to handle multiple requests simultaneously (no over-bookings)

Implementation Details:
- Instead of a real database, InMemory storage will be used (just a simplified simulation)
- The Command Line Interface (CLI) will be used as the UI

Available CLI commands:
- list_theaters: list all Threaters
- list_movies: list all Movies
- list_playing_movies: List all Playing movies
- find_theaters: List all Threaters playing specified Movie
- select_theater: Select the Theater by its name
- select_movie: Select Movie by its name
- list_available_seats: List all available seats for the selected movie at the selected cinema
- book_seats: Book available seats for the premiere


<a name="Download"></a>
## Download project:
- git clone https://github.com/AndreyTokmakov/BookingService.git
- cd BookingService

<a name="Requirements"></a>
## Project requirements
- C++20
- Boost Libraries (BoostTest framework - used for tests)

<a name="Build"></a>
## Build
- mkdir build
- cd build
- cmake -DCMAKE_BUILD_TYPE=Release ..

<a name="Tests"></a>
## How to run tests
- mkdir build
- cd build
- cmake -DCMAKE_BUILD_TYPE=Release ..
- ./test/tests