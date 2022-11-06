/*! @file main/Route.cpp
    @brief implementation of the Route class
*/

#include "Route.h"

/// @brief check if two lines intersect
bool Route::isIntersect(Line& L1, Line& L2)
{
  point vLp1, vLp2, hLp1, hLp2;
  if(L1.p1().first == L1.p2().first) {
    // L1 is vertical, assume that L2 is horizontal
    vLp1 = L1.p1();
    vLp2 = L1.p2();
    hLp1 = L2.p1();
    hLp2 = L2.p2();
  }else {
    // L2 is vertical, assume that L1 is horizontal
    vLp1 = L2.p1();
    vLp2 = L2.p2();
    hLp1 = L1.p1();
    hLp2 = L1.p2();
  }

  if(vLp1.first < std::min(hLp1.first, hLp2.first) || vLp1.first > std::max(hLp1.first, hLp2.first))
    return false;

  if((vLp1.second-hLp1.second) * (vLp2.second-hLp1.second) <= 0) {
    return true;
  }else {
    return false;
  }
}

/// @brief if a block edge and a line intersect, divide the line into two
///
/// @warning make sure first param is a block edge while second is the line to divide 
std::vector<Line> Route::divIntersect(Line& edge, Line& L)
{
  point intersection;

  if(edge.p1().first == edge.p2().first) {
    intersection.first = edge.p1().first;
    intersection.second = L.p1().second;
  }else {
    intersection.first = L.p1().first;
    intersection.second = edge.p1().second;
  }

  Line L1(L.pId()), L2(L.pId());

  L1.setP1(L.p1());
  L1.setP2(intersection);
  L2.setP1(intersection);
  L2.setP2(L.p2());

  std::vector<Line> lines = {L1, L2};
  return lines;
}

/// @brief check if two lines fit the constraint "d"
bool Route::isLegal(Line& L1, Line& L2)
{
  auto L1p1 = L1.p1();
  auto L1p2 = L1.p2();
  auto L2p1 = L2.p1();
  auto L2p2 = L2.p2();
  int d = 5;
  if(L1p1.first == L1p2.first) {
    // L1 is vertical
    if(L2p1.second == L2p2.second) {
      return true;
    }else {
      // L1 and L2 are vertical
      if(std::abs(L1p1.first-L2p1.first) > d) {
        return true;
      }else {
        // for point A(x1,y1) and B(x2,y2), if point C(x,y) is between A and B in Y direction projection, then (y-y1)*(y-y2) <=0
        if((L1p1.second-L2p1.second)*(L1p1.second-L2p2.second) <= 0 || (L1p2.second-L2p1.second)*(L1p2.second-L2p2.second) <= 0)
          return false;
        if((L2p1.second-L1p1.second)*(L2p1.second-L1p2.second) <= 0 || (L2p2.second-L1p1.second)*(L2p2.second-L1p2.second) <= 0) 
          return false;
      }
    }
  }else {
    // L1 is horizontal
    if(L2p1.first == L2p2.first) {
      return true;
    }else {
      // L1 and L2 are horizontal
      if(std::abs(L1p1.second-L2p1.second) > d) {
        return true;
      }else {
        // for point A(x1,y1) and B(x2,y2), if point C(x,y) is between A and B in X direction projection, then (x-x1)*(x-x2) <=0
        if((L1p1.first-L2p1.first)*(L1p1.first-L2p2.first) <= 0 || (L1p2.first-L2p1.first)*(L1p2.first-L2p2.first) <= 0) 
          return false;
        if((L2p1.first-L1p1.first)*(L2p1.first-L1p2.first) <= 0 || (L2p2.first-L1p1.first)*(L2p2.first-L1p2.first) <= 0) 
          return false;
      }
    }
  }
  return true;
}