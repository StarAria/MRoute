/*! @file main/main.cpp
    @brief main.cpp
    @date 2022/11/06

    Takes 1 argument input. 
    Parse the file into Route instance. 
    Generate routing result with no violation to the min space rule.
    Input file should be of certain format.
*/

#include "./Route.h"
#include<iostream>

int main(int argc, char* argv[])
{
    Route route;
    route.parser("input.txt");
    if(!route.run())
        std::cout << "Failed!" << std::endl;
    else
        std::cout << "All the flylines routed successfully!" << std::endl;
    route.output(std::string("output.txt"));

    return 0;
}

