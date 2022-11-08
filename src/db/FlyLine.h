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
    std::string         _nodeName1;
    std::string         _nodeName2;
    point               _p1;
    point               _p2;
    std::vector<Line>   _lines;

public:
    /*! @brief FlyLine coonstructer using id only. */
    FlyLine(int id): _id(id) {}
    /*! @brief FlyLine coonstructer using id and points. */
    FlyLine(int id, point p1, point p2, std::string nodeName1, std::string nodeName2)
        : _p1(p1), _p2(p2), _id(id), _nodeName1(nodeName1), _nodeName2(nodeName2) {}
    /*! @brief FlyLine coonstructer with complete data members. */
    FlyLine(int id, point p1, point p2, std::string nodeName1, std::string nodeName2, std::vector<Line> lines)
        : _p1(p1), _p2(p2), _id(id), _nodeName1(nodeName1), _nodeName2(nodeName2), _lines(lines) {}

    /*! @brief Get id of flyline instance. */
    int                 id() const {return _id;};
    /*! @brief Get the first point of flyline instance. */
    point               p1() const {return _p1;};
    /*! @brief Get the second point of flyline instance. */
    point               p2() const {return _p2;};
    /*! @brief Get the first node name of flyline instance. */
    std::string         name1() const {return _nodeName1;}
    /*! @brief Get the second node name of flyline instance. */
    std::string         name2() const {return _nodeName2;}
    /*! @brief Get lines of flyline instance. */
    std::vector<Line>   lines() const {return _lines;};

    /*! @brief Set the first point of flyline instant.Lines will be cleared */
    void                setP1(point p);
    /*! @brief Set the second point of flyline instant.Lines will be cleared */
    void                setP2(point p);
    /*! @brief Set the first node name of flyline instance. */
    void                setName1(std::string name);
    /*! @brief Set the second node name of flyline instance. */
    void                setName2(std::string name);
    /*! @brief Set the lines of flyline instant. */
    void                setLines(std::vector<Line> lines);

    /*! @brief Sort line in lines in order of connection from _p1 to _p2.
    Return false if failed or error occur.*/
    bool                sortLines();
    /*! @brief Add a line to _lines vector. */
    void                addLine(Line line);
    /*! @brief Print FlyLine info.Used for debugging. */
    void                printFlyline() const;

};

#endif