/*! @file db/FlyLine.cpp
    @brief FlyLine class implementation.
    @date 2022/11/04
*/

#include "./FlyLine.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

void FlyLine::setP1(point p)
{
    _p1 = p;
    _lines.clear();
    return;
}

void FlyLine::setP2(point p)
{
    _p2 = p;
    _lines.clear();
    return;
}

void FlyLine::setName1(std::string name)
{
    _nodeName1 = name;
    return;
}

void FlyLine::setName2(std::string name)
{
    _nodeName2 = name;
    return;
}

void FlyLine::setLines(std::vector<Line> lines)
{
    _lines.assign(lines.begin(), lines.end());
    return;
}

bool FlyLine::sortLines()
{
    point p = _p1;
    for(int index1 = 0; index1 < _lines.size(); index1++)
    {
        bool find = false;
        for(int index2 = index1; index2 < _lines.size(); index2++)
        {
            if(pointEqual(_lines[index2].p1(), p))
            {
                find = true;
                std::swap(_lines[index1], _lines[index2]);
                p = _lines[index1].p2();
                break;
            }
            else if(pointEqual(_lines[index2].p2(), p))
            {
                find = true;
                std::swap(_lines[index1], _lines[index2]);
                p = _lines[index1].p1();
                //swap _p1 _p2 to fit the order 
                _lines[index1].setP1(_lines[index1].p2());
                _lines[index1].setP2(p);
                break;
            }
        }
        if(!find)
            return false;   //Can't find the next line;
    }
    if(!pointEqual(p, _p2))
        return false;       //Flyline doesn't link to the target point _p2;
    return true;
}

void FlyLine::addLine(Line line)
{
    _lines.push_back(line);
    return;
}

void FlyLine::printFlyline() const
{
    std::cout << "id:" << _id << std::endl;
    std::cout << "start point:(" << _p1.first << ", " << _p1.second << ")" << std::endl;
    std::cout << "end point:(" << _p2.first << ", " << _p2.second << ")" << std::endl;
    std::cout << "lines:" << std::endl;
    for(Line line : _lines)
        std::cout << "(" << line.p1().first << ", " << line.p1().second << ")" << "\t" << "(" << line.p2().first << ", " << line.p2().second << ")" << std::endl;
    std::cout << std::endl;
    return;
}

void FlyLine::printLines(std::ofstream &fp)
{
    sortLines();
    fp << "<" << _nodeName1 << "> {";
    for(Line line : _lines)
        fp << "{" << std::fixed << std::setprecision(4) << line.p1().first 
           << " " << std::fixed << std::setprecision(4) << line.p1().second << "} ";
    fp << "{" << std::fixed << std::setprecision(4) << _p2.first 
       << " " << std::fixed << std::setprecision(4) << _p2.second << "}";
    fp << "} <" << _nodeName2 << ">" << std::endl;
    return;
}
