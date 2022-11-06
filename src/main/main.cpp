/*! @file main/main.cpp
    @brief main.cpp
    @date 2022/11/06

    Takes 1 argument input. 
    Parse the file into Route instance. 
    Generate routing result with no violation to the min space rule.
    Input file should be of certain format.
*/

#include "./Route.h"

int main(int argc, char* argv[])
{
    Route route;
    route.parser(std::string(argv[0]));
    route.run();
    if(argc > 1)
        route.output(std::string(argv[1]));
    else
        route.output(std::string("output.txt"));
    return 0;
}

