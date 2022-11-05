/*! @file main/Route.h
    @brief main class for routing problem
    @date 2022/11/30
*/

#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "../db/Block.h"
#include "../db/Type.h"
#include "../db/FlyLine.h"
#include "../db/Line.h"
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
        std::vector<int>        _instsId;
    };
    
    double                      d, w, h;
    double                      width, height;

    std::vector<BlockTemplate>  tempaltes;
    std::vector<Block>          blocks;
    std::vector<FlyLine>        flyLines;
    std::vector<Line>           vLines;
    std::vector<Line>           hLines;

public:
    Route() {};
    void parser(std::string);
    void output();
    bool isIntersect(Line&, Line&);
    std::vector<Line> divIntersect(Line&, Line&);
    bool isLegal(Line& L1, Line& L2);
};


#endif