/*! @file db/Type.h
    @brief Type header file.
    @date 2022/11/30
*/

#ifndef __TYPE_H__
#define __TYPE_H__

#include <utility>
#include <cstdint>
#include <cmath>

constexpr   double      delta = 0.00001;

/*! @brief Check if two double numbers equal within the error range delta.*/
inline bool approxEqual(double x, double y){return std::abs(x - y) < delta;}

/*! @class Point */
typedef std::pair<double, double> point;

/*! @brief Check if two points equal within the error range delta.*/
inline bool pointEqual(point p1, point p2){return approxEqual(p1.first, p2.first) && approxEqual(p1.second, p2.second);}

/*! @enum OriType 
    @brief Type of orientation*/

enum class OriType : std::uint8_t
{
    R0,
    MX,
    MY,
    R180
};

/*! @enum OriType 
    @brief Type of line*/
enum class LineType : std::uint8_t
{
    EDGE,
    FLYLINE,
    SUS,        //line with two suspended point
    HSUS,       //line with one routing point and one suspended point
};

enum class Pattern : std::uint8_t
{
    LB,         //"L"shape with bottom line
    LU,         //"L"shape with upper line
    H,          //"H"shape with two vertical lines and one horizonal line
    Z,          //"Z"shape with two horizonal lines and one vertical line
};

#endif