FROM ubuntu:latest

RUN echo "Resolving environment requirements: GCC-13, G++-13, CMake, C++ Boost Libraries"
RUN apt-get update && apt-get upgrade -y
RUN apt-get install software-properties-common dirmngr apt-transport-https lsb-release ca-certificates  -y
RUN add-apt-repository ppa:ubuntu-toolchain-r/test -y
RUN apt-get install gcc-13 g++-13 build-essential cmake make libboost-all-dev -y

RUN echo "Copying BookingService files to the container"
COPY . /service
RUN mkdir /service/build
WORKDIR /service/build

RUN echo "Configuring the G++ compilers environment variables for the CMake"
ENV CC=gcc-13 CXX=g++-13

RUN echo "Build and compile project..."
RUN cmake ..
RUN make -j4

RUN echo "Running unit-tests..."
RUN ./test/tests -l all
