/*! @file db/FlyLine.h
    @brief FlyLine header file.
    @date 2022/11/30
*/

#ifndef __FLYLINE_H__
#define __FLYLINE_H__

#include "./Type.h"
#include "./Line.h"
#include <vector>

/*! @class FlyLine
    @brief FlyLine class for two point flyline.
*/

class FlyLine
{
private:
    int                 _id;
    point               _p1;
    point               _p2;
    std::vector<Line&>  _lines;
};

#endif