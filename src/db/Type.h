/*! @file db/Type.h
    @brief Type header file.
    @date 2022/11/30
*/

#ifndef __TYPE_H__
#define __TYPE_H__

#include <utility>
#include <cstdint>

/*! @class Point */
typedef std::pair<double, double> point;

class Edge
{
private:
    std::pair<point, point>         line;
    int                             Id;
    int                             fId;
    bool                            type;
}


/*! @enum OriType 
    @brief Type of orientation*/

enum class OriType : std::uint8_t
{
    R0,
    MX,
    MY,
    R180
};

#endif