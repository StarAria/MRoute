/*! @file db/Grid.h
    @brief Grid header file.
    @date 2022/11/30
*/

#ifndef __GRID_H__
#define __GRID_H__

#include <vector>
#include <utility>

/*! @class Grid
    @brief Grid net for routing problem.
*/
class Grid
{
private:
    struct GridNode
    {
        int                 _id;
        int                 _left, _right, _up, _down;
        double              lCost, rCost, uCost, dCost;
    };

    struct GridLine
    {
        int                 _startId, _endId;
        std::vector<int>    _routePoints;
    };
    
    int                     xRange, yRange;
    std::vector<GridNode>   gridNodes;
    std::vector<GridLine>   lines;
    
public:
   
};


#endif