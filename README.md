# BookingService
Demo BookingService 

1. [Overview](#Overview)
2. [Download](#Download)
3. [Build](#Build)
   1. [Linux](#linux_build)
   2. [Windows](#windows_build)
4. [Requirements](#Requirements)
5. [Run CLI](#Run_CLI)
6. [How to run tests](#Tests)
7. [How to use CLI](#CLI)

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
- q: Exit/Close CLI


<a name="Download"></a>
## Download project:
- Copy/clone the project repo: `git clone https://github.com/AndreyTokmakov/BookingService.git`
- move to the project folder: `cd BookingService`

<a name="Requirements"></a>
## Project requirements
- C++20
- Cmake
- Boost Libraries (BoostTest framework - used for tests)

<a name="Build"></a>
## Build
<a name="linux_build"></a>
### Linux 
- Create build folder: `mkdir build`
- Move to the created directory: `cd build`
- Configure project build (with cmake): `cmake -DCMAKE_BUILD_TYPE=Release ..`
- Build and compile solution: `make`,
  alternatively, the number of CPUs can be specified to speed up the build `make -j8`

<a name="windows_build"></a>
### Windows
- Same steps as when building for Linux 
- You only need the **BOOST_ROOT** variable to configure the project for **Cmake**

<a name="Run_CLI"></a>
## Run CLI
- Move to the build folder: `cd build`
- Start CLI application: `./src/BookingService`


<a name="Tests"></a>
## How to run tests
- Move to the build folder: `cd build`
- Run the tests: `./test/tests -p`
```
Running 36 test cases...

0%   10   20   30   40   50   60   70   80   90   100%
|----|----|----|----|----|----|----|----|----|----|
***************************************************

*** No errors detected
```

<a name="CLI"></a>
## How to use CLI
A basic example of using the CLI by a user:
1. View available Movies and search for the right Theater
2. Choosing a Theater and a Movie
3. Getting a list of places available for booking
4. Booking the desired seats (if available)
```
> list_playing_movies
	Inception
	Terminator
	The Green Mile
	Fight Club

> find_theaters Terminator
The movie 'Terminator' is being shown in:
	4DX

> select_movie Terminator
The Terminator movie is chosen

> select_theater 4DX
The 4DX theater is chosen

> list_available_seats
Theater: 4DX, Movie : Terminator
Seats available: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20

> book_seats 1,2,3,4,5
Seats 1,2,3,4,5 are booked

> list_available_seats
Theater: 4DX, Movie : Terminator
Seats available: 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 

> book_seats 4        
Sorry: Failed to book seats: 4

> q
```
