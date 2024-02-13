# decentralized-messaging-system

This repository contains the source code for a decentralized messenger built on a microservices architecture, utilizing various technologies for efficient data processing and ensuring security. The project is still under active development.

## Key Features

- **Microservices Architecture:** Implemented a decentralized messenger using microservices to enhance scalability and manage various components of the system.

- **Go Language and Database Interaction:** Go programming language is employed for interacting with the database using the Gorm ORM. The database is accessible through a REST API using Gin, with documentation developed using Swagger.

- **Redis Caching:** User addresses and nicknames are cached using Redis, with interaction facilitated through the hiredis library.

- **Security:** All classes accessing data storage are implemented with thread-safety in mind. The authentication service is realized using JWT (Access + Refresh) stored in MongoDB with Go.

- **Service Interaction:** Services communicate with each other using the gRPC protocol, ensuring efficient data exchange.

- **Boost.Beast and gRPC:** Boost.Beast is used for sending requests to the database, and gRPC facilitates data exchange between services.

- **Tools and Technologies:** The project employs various tools and technologies, including Docker, docker-compose, Swagger, pgAdmin4, plantUML, and GitHub Actions.

## Technologies and Libraries

- **Backend (Go):** Go, Gorm, PostgreSQL, Gin, mongo-go-driver, bcrypt
- **Backend (C++):** C++, CMake, GTest, Hiredis, Boost, Conan
- **Protocols and Tools:** gRPC, Protobuf, Docker, docker-compose, Swagger, pgAdmin4, plantUML, GitHub Actions

## Running the Project

The development process is ongoing, and details on running the project will be provided in future updates.

## License

This project is licensed under the terms of the [MIT License](LICENSE).
