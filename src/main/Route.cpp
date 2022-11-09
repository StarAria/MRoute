/*! @file main/Route.cpp
    @brief implementation of the Route class
*/

#include "Route.h"

using std::vector;

bool Route::run()
{
  vector<Line> buffer;
  //TODO:generate biasRange and step.
  double biasRange = 0, step = 0, bias = 0;
  for(FlyLine & flyline : flylines)
  {
    //try LB shape
    buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::LB);
    if(syncAndCheck(buffer))
    {
      flyline.setLines(buffer);
      flyline.sortLines();
      continue;
    }
    //try LU shape
    buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::LU);
    if(syncAndCheck(buffer))
    {
      flyline.setLines(buffer);
      flyline.sortLines();
      continue;
    }
    //try H and Z shape with bias and -bias in one cycle
    for(bias = 0; bias < biasRange; bias += step)
    {
      //try H shapes with bias
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::H, bias);
      if(syncAndCheck(buffer))
      {
        flyline.setLines(buffer);
        flyline.sortLines();
        break;
      }
      //try H shapes with -bias
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::H, (-bias));
      if(syncAndCheck(buffer))
      {
        flyline.setLines(buffer);
        flyline.sortLines();
        break;
      }
      //try Z shapes with bias
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::Z, bias);
      if(syncAndCheck(buffer))
      {
        flyline.setLines(buffer);
        flyline.sortLines();
        break;
      }
      //try Z shapes with -bias
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::Z, (-bias));
      if(syncAndCheck(buffer))
      {
        flyline.setLines(buffer);
        flyline.sortLines();
        break;
      }
    }
      if(bias < biasRange)    //find solution with H or Z shape
        continue;
      return false;           //cannot find solution in specific range
  }
  return true;                //all the flylines have been routed successfully
}


bool Route::isIntersect(Line& L1, Line& L2)
{
  point vLp1, vLp2, hLp1, hLp2;
  if(approxEqual(L1.p1().first, L1.p2().first)) {
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

  if(vLp1.first <= std::min(hLp1.first, hLp2.first) || vLp1.first >= std::max(hLp1.first, hLp2.first))
    return false;
  
  if(approxEqual(vLp1.second, hLp1.second) || approxEqual(vLp2.second, hLp1.second))
    return false;

  if((vLp1.second-hLp1.second) * (vLp2.second-hLp1.second) < 0) {
    return true;
  }else {
    return false;
  }
}

vector<Line> Route::divIntersect(Line& edge, Line& L)
{
  point intersection;

  if(approxEqual(edge.p1().first, edge.p2().first)) {
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

  vector<Line> lines = {L1, L2};
  return lines;
}

bool Route::isLegal(Line& L1, Line& L2)
{
  auto L1p1 = L1.p1();
  auto L1p2 = L1.p2();
  auto L2p1 = L2.p1();
  auto L2p2 = L2.p2();

  // L1 is vertical
  if(approxEqual(L1p1.first, L1p2.first)) {
    // L2 is horizontal
    if(approxEqual(L2p1.second, L2p2.second)) {
      return true;
    }else {
      // L2 is vertical
      if(approxEqual(L1p1.first, L2p1.first)) {
        if((L1p1.second - L2p2.second) > d || (L2p1.second - L1p2.second) >d){
          return true;
        }else {
          return false;
        }
      }
      if(std::abs(L1p1.first-L2p1.first) > d) {
        return true;
      }else {
        // for point A(x1,y1) and B(x2,y2), if point C(x,y) is between A and B in Y direction projection, then (y-y1)*(y-y2) <=0
        if((L1p1.second-L2p1.second-d)*(L1p1.second-L2p2.second-d) <= 0 || (L1p2.second-L2p1.second-d)*(L1p2.second-L2p2.second-d) <= 0)
          return false;
        if((L2p1.second-L1p1.second-d)*(L2p1.second-L1p2.second-d) <= 0 || (L2p2.second-L1p1.second-d)*(L2p2.second-L1p2.second-d) <= 0) 
          return false;
      }

    }
  // L1 is horizontal
  }else {
    // L1 is vertical
    if(approxEqual(L2p1.first, L2p2.first)) {
      return true;
    }else {
      // L2 is horizontal
      if(approxEqual(L1p1.second, L2p1.second)) {
        if((L1p1.first - L2p2.first) > d || (L2p1.first - L1p2.first) >d){
          return true;
        }else {
          return false;
        }
      }
      if(std::abs(L1p1.second-L2p1.second) > d || approxEqual(L1p1.second, L2p1.second)) {
        return true;
      }else {
        // for point A(x1,y1) and B(x2,y2), if point C(x,y) is between A and B in X direction projection, then (x-x1)*(x-x2) <=0
        if((L1p1.first-L2p1.first-d)*(L1p1.first-L2p2.first-d) <= 0 || (L1p2.first-L2p1.first-d)*(L1p2.first-L2p2.first-d) <= 0) 
          return false;
        if((L2p1.first-L1p1.first-d)*(L2p1.first-L1p2.first-d) <= 0 || (L2p2.first-L1p1.first-d)*(L2p2.first-L1p2.first-d) <= 0) 
          return false;
      }
    }
  }
  return true;
}

bool Route::syncAndCheck(vector<Line>& buffer)
{
  for(Line &L1 : buffer) {
    for(Line &L2 : Lines) {
      if(isLegal(L1, L2) == 0)
        return false;
    }
  }

  // tmpBuf holds lines which can not be divided
  vector<Line> tmpBuf(buffer);
  for(auto &block : blocks) {
    for(auto &edge : block.edges()) {

      for(auto it = tmpBuf.begin(); it != tmpBuf.end();) {
        if(isIntersect(edge, *it)) {
          auto t = divIntersect(edge, *it);
          it = tmpBuf.erase(it);
          tmpBuf.insert(tmpBuf.end(), t.begin(), t.end());
        }else {
          ++it;
        }
      }

    }
  }

  vector<Line> tmpLs(Lines);
  Line syncL;
  for(auto &line : tmpBuf) {
    for(auto &block : blocks) {
      if(block.judgeInside(line.p1()) && block.judgeInside(line.p2())) {
        auto tmpL1 = line.flip(block.ori(), block.bias());
        auto blockIds = templates[block.templateId()]._instsId;
        for(auto id : blockIds){
          if(id == block.id())
            continue;
          
          double xbias = blocks[id].bias().first - block.bias().first;
          double ybias = blocks[id].bias().second - block.bias().second;

          point p1, p2;
          p1.first = tmpL1.p1().first + xbias;
          p1.second = tmpL1.p1().second + ybias;
          p2.first = tmpL1.p2().first + xbias;
          p2.second = tmpL1.p2().second + ybias;
          Line tmpL2(-1, p1, p2);
          syncL = tmpL2.flip(blocks[id].ori(), blocks[id].bias());

          int flag = 0;
          for(auto tL : tmpLs) {
            if(isLegal(syncL, tL)) {
              flag = 1;
              break;
            }
          }
          if(flag == 0)
            tmpLs.push_back(syncL);
        }
      }
    }
  }

  for(Line &L1 : buffer) {
    for(Line &L2 : tmpLs) {
      if(isLegal(L1, L2) == 0)
        return false;
    }
  }

  for(Line &L : buffer){
    L.setId(static_cast<int>(tmpLs.size()));
    tmpLs.push_back(L);
  }
  Lines = tmpLs;
  return true;
}

