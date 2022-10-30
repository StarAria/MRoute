/*! @file main/Route.h
    @brief main class for routing problem
    @date 2022/11/30
*/

#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "../db/Grid.h"
#include "../db/Block.h"
#include "../db/Node.h"
#include "../db/Net.h"
#include <vector>
#include <string>

/*! @class Route
    @brief main class to solve routing problem
*/
class Route
{
private:
    struct BlockTemplate
    {
        std::string             _name;
        std::vector<point>      _points;
        std::vector<Node>       _nodes;
        std::vector<int>        _instsId;
    };
    
    double                      d, w, h;
    double                      width, height;
    double                      gridLength;
    int                         gridBiasX, gridBiasY;

    Grid &                      grid;
    std::vector<BlockTemplate>  tempaltes;
    std::vector<Block>          blocks;
    std::vector<Net>            nets;
    std::vector<Node>           nodes;

};


#endif