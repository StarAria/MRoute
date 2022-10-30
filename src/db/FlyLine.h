/*! @file db/FlyLine.h
    @brief FlyLine header file.
    @date 2022/11/30
*/

#ifndef __FLYLINE_H__
#define __FLYLINE_H__

#include "./Type.h"
#include "./Node.h"
#include <vector>

/*! @class FlyLine
    @brief FlyLine class for two point flyline.
*/

class FlyLine
{
private:
    int                 _id;
    int                 _netId;
    int                 _startNodeId;
    int                 _endNodeId;
    point               _start;
    point               _end;
    std::vector<point>  _corner;
};

#endif