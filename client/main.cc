#include <iostream>

#include <core.h>

int main(int argc, char *argv[])
{
    RunServer server(argv[1], std::stoi(argv[2]));
    server.run();


    return 0;
}