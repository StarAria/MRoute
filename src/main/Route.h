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

    std::vector<BlockTemplate>  templates;
    std::vector<Block>          blocks;
    std::vector<FlyLine>        flylines;
    std::vector<Line>           Lines;

public:
    Route() {};

    /// @brief Function for read files to construct Route data.
    /// @param file Input file name.
    void parser(std::string file);
    /// @brief Function for generate result file.
    /// @param file Input file name.
    void output(std::string file);
    /// @brief run routing algorithm.Return false if fail;
    bool run();

    //TODO:Sort vLines, hLines and optimize design rule check algorithm.
    //TODO:Sort flylines with a certain order, to optimize routing algorithm.
    //TODO:Take SUS and HSUS lines into consideration.

    /*! @brief Generate lines pattern for pattern match routing algorithm.
        @param p1 First point for routing.
        @param p2 Second point for routing.
        @param p Pattern shape with "LB", "LU", "H", "Z" as options. 
        @param bias Vertical/Horizonal distance from midline for "H"/"Z"shape.Invalid on "LB" or "LU" shapes.
    */
    std::vector<Line> genPattern(point p1, point p2, Pattern p, double bias = 0.0);
    /*! @brief Sync for multiple instance module.
        Check with constraint "d" for all the new generateded lines.
        Return True and add lines to vLines and hLines if pass.
        Return false if not pass.
        @param buffer
    */
    bool syncAndCheck(std::vector<Line>& buffer);
    /// @brief check if two lines intersect
    bool isIntersect(Line&, Line&);
    /// @brief if a block edge and a line intersect, divide the line into two
    /// @warning make sure first param is a block edge while second is the line to divide 
    std::vector<Line> divIntersect(Line&, Line&);
    /// @brief check if two lines fit the constraint "d"
    bool isLegal(Line& L1, Line& L2);
    
    ///@brief form block's edges in parser using bias and oriType
    void blockedgeform(Block& b);
};


#endif
