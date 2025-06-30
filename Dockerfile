# Start from a clean Ubuntu OS

FROM ubuntu:22.04

# Install build tools and dependencies : compiler and CMake
RUN apt-get update &&\
    apt-get install -y g++ cmake make &&\
    apt-get clean


# Creating a work directory inside container
WORKDIR /app


# Copy project files into the image
COPY . .

# Build the project using cmake
RUN mkdir build && cd build && cmake .. && make


# Run the compiled server
CMD ["./build/webserver"]
