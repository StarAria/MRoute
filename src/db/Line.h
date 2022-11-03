/*! @file db/line.h
    @brief Line header file.
    @date 2022/11/30
*/

#ifndef __LINE_H__
#define __LINE_H__

#include <vector>
#include <./Type.h>

/*! @class Line
    @brief Line net for routing problem.
*/
class Line
{
private:
    int                         id;
    int                         pId;
    point                       p1;
    point                       p2;
    LineType                    type;
public:
   
};


#endif