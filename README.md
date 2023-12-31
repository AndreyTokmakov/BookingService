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
8. [Continuous Integration](#CI)
9. [Documentation](#Documentation)
10. [Docker environment](#docker)

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

**Implementation Details**:
- Instead of a real database, InMemory storage will be used (just a simplified simulation)
- The Command Line Interface (CLI) will be used as the UI

**Available CLI commands**:

| Command                | Description                                                            | Example                                 |
|------------------------|------------------------------------------------------------------------|-----------------------------------------|
| _list_theaters_        | Display the list all Theaters                                          | list_theaters                           |
| _list_movies_          | Display the list all Movies                                            | list_movies                             |
| _list_playing_movies_  | List all Playing movies                                                | list_playing_movies                     |
| _find_theaters_        | List all Theaters playing specified Movie                              | find_theaters Terminator                |
| _select_theater_       | Select the Theater by its name                                         | select_theater 4DX                      |
| _select_movie_         | Select Movie by its name                                               | select_movie Terminator                 |
| _list_available_seats_ | List all available seats for the selected movie at the selected cinema | list_available_seats                    |
| _book_seats_           | Book available seats for the premiere                                  | book_seats 1,2,3,4,5 <br/> book_seats 1 |
| _q_                    | Exit/Close CLI                                                         | q                                       |


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
   </br></br>![UnitTestsLocal](https://github.com/AndreyTokmakov/BookingService/blob/metadata/images/Unit_Test_local.png)
 

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

<a name="CI"></a>
## Continuous Integration
**GitHub Actions** is used for Quality Assurance purposes and to receive a rapid 
feedback on the status of the project and the presence/absence of kind of regression.
1. BookingService recent [workflows](https://github.com/AndreyTokmakov/BookingService/actions) status <br />
2. Builds and tests are configured to run on each code pushes to the **main**, **develop**
branches or on submitting Pull Request to the **main** branch
3. At each launch, the build status of the project is checked and Unit tests are run
   </br></br>![workflows](https://github.com/AndreyTokmakov/BookingService/blob/metadata/images/Actions_1.png)
   </br></br>![Unit-tests status](https://github.com/AndreyTokmakov/BookingService/blob/metadata/images/Actions_2.png)
   </br></br>![Pipeline](https://github.com/AndreyTokmakov/BookingService/blob/metadata/images/Actions_3.png)


<a name="Documentation"></a>
## Documentation
The project supports auto-documentation based on the Doxygen framework.
1. Install Doxygen (check version to make sure of installation) `doxygen --version`
2. Move to the project root folder and generate documentation `doxygen Doxyfile.in`
3. Open _./docs/html/index.html_ in Browser
Documentation views examples:
   </br></br>![namespaces](https://github.com/AndreyTokmakov/BookingService/blob/metadata/images/Doc_1.png)
   </br></br>![classes](https://github.com/AndreyTokmakov/BookingService/blob/metadata/images/Doc_2.png)
   </br></br>![graphs](https://github.com/AndreyTokmakov/BookingService/blob/metadata/images/Doc_3.png)

<a name="docker"></a>
## Docker environment
The option to run the service (application) and unit-tests in the docker container is also supported </br>
This option (~~In fact, Docker is completely unnecessary for the operation of the application and was created 
only for the purpose of demonstration~~) if its not desired to change the environment on a working OS
1. _How to build the docker image_: 
   1. Move to the project root folder
   2. build image: `docker build -t service_image . `</br></br>
2. _How to understand if the image has been  build successfully_:
   </br>If such a message appears at the end of the build process, then everything is OK
   ```
   *** No errors detected
   
   /service/test/main.cpp(194): Leaving test suite "CLI_Basic_Tests"; testing time: 699us
   Leaving test module "BookingServiceUnitTests"; testing time: 840us
   Removing intermediate container 89bf5903d653
   ---> 91004d63dabc
   Successfully built 91004d63dabc
   Successfully tagged service_image:latest
   ```
  
3. _To run service inside the container execute the following command_: </br>
  `docker run --rm -it --name service_container service_image ./src/BookingService`

**P.S** Docker installs all required components during build and runs unit-test-s. </br>
And only if all the steps are completed successfully, the image will be assembled. </br>
In case of an error when running tests, there will be a lot of red error messages in the console
