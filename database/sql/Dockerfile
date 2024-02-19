FROM golang:1.21-alpine 

RUN mkdir build
WORKDIR /build

COPY ./go_mod .
COPY ./go_sum .

RUN go mod download
