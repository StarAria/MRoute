/*! @file db/line.h
    @brief Line header file.
    @date 2022/11/30
*/

#ifndef __LINE_H__
#define __LINE_H__

#include <vector>
#include <string>

#include "Type.h"

/*! @class Line
    @brief Line net for routing problem.
*/
class Line
{
private:
    int                         _id;
    int                         _pId;
    point                       _p1;
    point                       _p2;
    LineType                    _type;

public:

    /// @brief constructor using pId
    Line(int pId): _pId(pId) {}

    /// @brief destructor
    ~Line() {}

    /// these function get specific data members 

    int id() const {return _id;}
    int pId() const {return _pId;}
    point p1() const {return _p1;}
    point p2() const {return _p2;}
    LineType type() const {return _type;}

    /// these function set specific data members 

    void setId(int id) {_id = id;}
    void setP1(point p1) {_p1 = p1;}
    void setP2(point p2) {_p2 = p2;}
    void setType(LineType type) {_type = type;}


};


#endif