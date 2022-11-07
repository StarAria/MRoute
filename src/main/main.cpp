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
    if(argc < 2) {
        std::cout << "need an input_file" << std::endl;
    }
    route.parser(argv[1]);
    route.run();
    route.output(std::string("output.txt"));

    return 0;
}

