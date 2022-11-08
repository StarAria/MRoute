/*! @file db/Line.cpp
    @brief implimentation of Line class.
*/

#include "Line.h"

Line::Line(int id, point p1, point p2)//make sure p1<p2
{
  _id = id;
  if (p1.first == p2.first) //vertical
  {
    if (p1.second > p2.second)
    {
      point t;
      t = p1;
      p1 = p2;
      p2 = t;
    }
  }
  else//horizonal
    if (p1.first > p2.first)
    {
      point t;
      t = p1;
      p1 = p2;
      p2 = t;
    }
  _p1 = p1;
  _p2 = p2;
  _pId = 0;
}

Line Line::flip(OriType type, point bias)
{
  Line tmpL(-1);
  point p1, p2;
  if(type == OriType::R0)
    return *this;
  
  if(type == OriType::MX) {
    p1.first = this->p1().first;
    p1.second = 2*bias.second - this->p1().second;
    p2.first = this->p2().first;
    p2.second = 2*bias.second - this->p2().second;
  }

  if(type == OriType::MY) {
    p1.first = 2*bias.first - this->p1().first;
    p1.second = this->p1().first;
    p2.first = 2*bias.first - this->p2().first;
    p2.second = this->p2().first;
  }

  if(type == OriType::R180) {
    p1.first = 2*bias.first - this->p1().first;
    p1.second = 2*bias.second - this->p1().second;
    p2.first = 2*bias.first - this->p2().first;
    p2.second = 2*bias.second - this->p2().second;
  }

  tmpL.setP1(p1);
  tmpL.setP2(p2);
  return tmpL;
}