FROM debian:bullseye

WORKDIR /usr/server/ 

RUN apt-get update && \
    apt-get install -y \
    build-essential \
    ninja-build \
    clang \
    cmake \
    python3 \ 
    python3-pip 

RUN pip3 install conan

COPY . .