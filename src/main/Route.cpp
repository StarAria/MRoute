/*! @file main/Route.cpp
    @brief implementation of the Route class
*/

#include "Route.h"
#include <fstream>
#include <iomanip>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

bool Route::run()
{
  double step = stepFactor * d;   //step:based on d
  double biasRange;
  for(FlyLine & flyline : flylines)
  {
    //biasRange:based on HPWL
    biasRange = biasRangeFactor * 
      (std::abs(flyline.p1().first - flyline.p2().first) + 
       std::abs(flyline.p1().second - flyline.p2().second));
    
    if(approxEqual(flyline.p1().first, flyline.p2().first))
    {
      std::cout << "TRACE" << std::endl;
      if(!vMatchRoute(flyline.p1(), flyline.p2(), step, biasRange, flyline))     //horizonal flyline
        return false;
    }                       
    else if(approxEqual(flyline.p1().second, flyline.p2().second))     //vertical flyline
    {
      if(!hMatchRoute(flyline.p1(), flyline.p2(), step, biasRange, flyline))
        return false;
    }
    else                                                               //common flyine
    {
      if(!lMatchRoute(flyline.p1(), flyline.p2(), step, biasRange, flyline))
        return false;
    }
  }

  return true;                //all the flylines have been routed successfully
}

bool Route::lMatchRoute(point p1, point p2, double step, double biasRange, FlyLine & flyline)
{
  vector<Line> buffer;
  //try LB shape
  buffer = genPattern(p1, p2, Pattern::LB);
  if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
  {
    flyline.setLines(buffer);
    return true;
  }
  //try LU shape
  buffer = genPattern(p1, p2, Pattern::LU);
  if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
  {
    flyline.setLines(buffer);
    return true;
  }
  //try H and Z shape with bias and -bias, HP and ZP shape with abs(bias1)+abs(bias2)=bias in one cycle.
  //bias = step * stepCount
  for(int stepCount = 0; step * stepCount < biasRange; stepCount++)
  {
    //try H shapes with bias
    buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::H, step * stepCount);
    if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
    {
      flyline.setLines(buffer);
      return true;
    }
    //try H shapes with -bias
    buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::H, (-step * stepCount));
    if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
    {
      flyline.setLines(buffer);
      return true;
    }
    //try Z shapes with bias
    buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::Z, step * stepCount);
    if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
    {
      flyline.setLines(buffer);
      return true;
    }
    //try Z shapes with -bias
    buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::Z, (-step * stepCount));
    if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
    {
      flyline.setLines(buffer);
      return true;
    }
    //try HP and ZP shape with abs(bias1)+abs(bias2)=bias
    for(int bias1Count = 0; bias1Count <= stepCount; bias1Count++)
    {
      //try HP pattern
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::HP, step * bias1Count, step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::HP, step * bias1Count, -step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::HP, -step * bias1Count, step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::HP, -step * bias1Count, -step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }

      //try ZP pattern
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::ZP, step * bias1Count, step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::ZP, step * bias1Count, -step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::ZP, -step * bias1Count, step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::ZP, -step * bias1Count, -step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
    }
  }
  return false;
}

bool Route::hMatchRoute(point p1, point p2, double step, double biasRange, FlyLine & flyline)
{
  vector<Line> buffer;
  //direct link
  buffer.clear();
  buffer.push_back(Line(-1, p1, p2));
  buffer[0].setpId(-1);
  if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
  {
    flyline.setLines(buffer);
    return true;
  }

  //try H shape with bias and -bias, HP and ZP shape with abs(bias1)+abs(bias2)=bias in one cycle.
  //skip all the Z shape, H shape with bias = 0.0, HP and LP shape with bias2 = 0.0
  //bias = step * stepCount
  for(int stepCount = 1; step * stepCount < biasRange; stepCount++)
  {
    //try H shapes with bias
    buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::H, step * stepCount);
    if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
    {
      flyline.setLines(buffer);
      return true;
    }
    //try H shapes with -bias
    buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::H, (-step * stepCount));
    if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
    {
      flyline.setLines(buffer);
      return true;
    }

    //try HP and ZP shape with abs(bias1)+abs(bias2)=bias
    for(int bias1Count = 0; bias1Count < stepCount; bias1Count++)
    {
      //try HP pattern
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::HP, step * bias1Count, step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::HP, step * bias1Count, -step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::HP, -step * bias1Count, step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::HP, -step * bias1Count, -step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }

      //try ZP pattern
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::ZP, step * bias1Count, step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::ZP, step * bias1Count, -step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::ZP, -step * bias1Count, step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::ZP, -step * bias1Count, -step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
    }
  }
  return false;
}

bool Route::vMatchRoute(point p1, point p2, double step, double biasRange, FlyLine & flyline)
{
  vector<Line> buffer;
  //direct link
  buffer.clear();
  buffer.push_back(Line(-1, p1, p2));
  buffer[0].setpId(-1);
  if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
  {
    flyline.setLines(buffer);
    return true;
  }

  //try Z shape with bias and -bias, HP and ZP shape with abs(bias1)+abs(bias2)=bias in one cycle.
  //skip all the H shape, Z shape with bias = 0.0, HP and LP shape with bias1 = 0.0
  //bias = step * stepCount
  for(int stepCount = 1; step * stepCount < biasRange; stepCount++)
  {
    //try Z shapes with bias
    buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::Z, step * stepCount);
    if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
    {
      flyline.setLines(buffer);
      return true;
    }
    //try Z shapes with -bias
    buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::Z, (-step * stepCount));
    if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
    {
      flyline.setLines(buffer);
      return true;
    }

    //try HP and ZP shape with abs(bias1)+abs(bias2)=bias
    for(int bias1Count = 1; bias1Count <= stepCount; bias1Count++)
    {
      //try HP pattern
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::HP, step * bias1Count, step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::HP, step * bias1Count, -step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::HP, -step * bias1Count, step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::HP, -step * bias1Count, -step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }

      //try ZP pattern
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::ZP, step * bias1Count, step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::ZP, step * bias1Count, -step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::ZP, -step * bias1Count, step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
      buffer = genPattern(flyline.p1(), flyline.p2(), Pattern::ZP, -step * bias1Count, -step * (stepCount - bias1Count));
      if(syncAndCheck(buffer, flyline.p1(), flyline.p2()))
      {
        flyline.setLines(buffer);
        return true;
      }
    }
  }
  return false;
}

std::vector<Line> Route::genPattern(point p1, point p2, Pattern p, double bias1, double bias2)
{
  
  std::vector<point> buffer;
  std::vector<Line> lines;
  double tmp1, tmp2;
  point pre;
  //generate corner points
  if(p == Pattern::LB)
  {
    if(p1.second < p2.second)
      buffer.push_back(point(p2.first, p1.second));
    else
      buffer.push_back(point(p1.first, p2.second));
  }
  else if(p == Pattern::LU)
  {
    if(p1.second < p2.second)
      buffer.push_back(point(p1.first, p2.second));
    else
      buffer.push_back(point(p2.first, p1.second));
  }
  else if(p == Pattern::H)
  {
    tmp2 = (p1.second + p2.second) / 2 + bias1;
    buffer.push_back(point(p1.first, tmp2));
    buffer.push_back(point(p2.first, tmp2));
  }
  else if(p == Pattern::Z)
  {
    tmp1 = (p1.first + p2.first) / 2 + bias1;
    buffer.push_back(point(tmp1, p1.second));
    buffer.push_back(point(tmp1, p2.second));
  }
  else if(p == Pattern::HP)
  {
    tmp1 = (p1.first + p2.first) / 2 + bias1;
    tmp2 = (p1.second + p2.second) / 2 + bias2;
    buffer.push_back(point(p1.first, tmp2));
    buffer.push_back(point(tmp1, tmp2));
    buffer.push_back(point(tmp1, p2.second));
  }
  else if(p == Pattern::ZP)
  {
    tmp1 = (p1.first + p2.first) / 2 + bias1;
    tmp2 = (p1.second + p2.second) / 2 + bias2;
    buffer.push_back(point(tmp1, p1.second));
    buffer.push_back(point(tmp1, tmp2));
    buffer.push_back(point(p2.first, tmp2));
  }

  //generate lines
  buffer.push_back(p2);
  pre = p1;
  for(point pt : buffer)
  {
    //id and pid unknown yet, set to -1.
    Line line(-1, pre, pt);
    line.setpId(-1);
    lines.push_back(line);
    pre = pt;
  }
  return lines;
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

  if(approxEqual(vLp1.first, hLp1.first) || approxEqual(vLp1.first, hLp2.first))
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

  Line L1(-1, L.p1(), intersection), L2(-1, L.p2(), intersection);
  vector<Line> lines = {L1, L2};
  return lines;
}

bool Route::lineLegal(Line& L1, Line& L2)
{
  auto L1p1 = L1.p1();
  auto L1p2 = L1.p2();
  auto L2p1 = L2.p1();
  auto L2p2 = L2.p2();

  if(pointEqual(L1p1, L2p1) || pointEqual(L1p1, L2p2) || pointEqual(L1p2, L2p1) || pointEqual(L1p2, L2p2))
    return true;

   // L1 is vertical
  if(approxEqual(L1p1.first, L1p2.first)) {
    // L2 is horizontal
    if(approxEqual(L2p1.second, L2p2.second)) {
      return true;
    }else {
      // L2 is vertical
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
    return true;
  }
  // L1 is horizontal
  else {
    // L1 is vertical
    if(approxEqual(L2p1.first, L2p2.first)) {
      return true;
    }else {
      // L2 is horizontal
      if(std::abs(L1p1.second-L2p1.second) > d) {
        return true;
      }else {
        // for point A(x1,y1) and B(x2,y2), if point C(x,y) is between A and B in X direction projection, then (x-x1)*(x-x2) <=0
        if((L1p1.first-L2p1.first-d)*(L1p1.first-L2p2.first-d) <= 0 || (L1p2.first-L2p1.first-d)*(L1p2.first-L2p2.first-d) <= 0) 
          return false;
        if((L2p1.first-L1p1.first-d)*(L2p1.first-L1p2.first-d) <= 0 || (L2p2.first-L1p1.first-d)*(L2p2.first-L1p2.first-d) <= 0) 
          return false;
      }
    }
    return true;
  }
}


bool Route::syncAndCheck(vector<Line>& buffer, point p1, point p2)
{
  for(Line &L1 : buffer) {
    if(pointLegal(L1, p1, p2) == 0){
      return false;
    }
      
    for(Line &L2 : Lines) {
      if(lineLegal(L1, L2) == 0)
        return false;
    }
  }

  // tmpBuf holds lines which can not be divided
  vector<Line> tmpBuf(buffer);
  for(auto &block : blocks) {
    for(auto &edge : block.edges()) {

      for(int i = 0; i != tmpBuf.size();) {
        if(isIntersect(edge, tmpBuf[i])) {
          auto t = divIntersect(edge, tmpBuf[i]);
          tmpBuf.erase(tmpBuf.begin() + i);
          tmpBuf.insert(tmpBuf.end(), t.begin(), t.end());
        }else {
          ++i;
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

          if(pointLegal(syncL) == 0)
            return false;
          
          int flag = 1;
          for(auto tL : tmpLs) {
            if(lineLegal(syncL, tL) == 0) {
              return false;
            if(pointEqual(tL.p1(), syncL.p1()) && pointEqual(tL.p2(), syncL.p2()))
              flag = 0;
            }
          }
          if(flag == 1) {
            tmpLs.push_back(syncL);
          }         
        }
      }
    }
  }

  for(Line &L1 : buffer) {
    for(Line &L2 : tmpLs) {
      if(lineLegal(L1, L2) == 0)
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



std::string tokenizer(std::string& line, const std::string& delims)
{
	std::string toke;

	// find the beginning position of first token
	std::string::size_type idx_begin = line.find_first_not_of(delims);

	if (idx_begin != std::string::npos) {
		std::string::size_type idx_end = line.find_first_of(delims, idx_begin);

		// last word
		if (idx_end == std::string::npos) {
			idx_end = line.length();
		}

		// extract the first token and erase it from the input string
		toke = line.substr(idx_begin, idx_end - idx_begin);
		line.erase(0, idx_end - 0);
	} // end if

	return toke;
}

void Route::blockedgeform(Block& b)
{
	std::vector<point> vertexes;
	vertexes.push_back(b.bias());
	if (b.ori() == OriType::R0) {
		for (int i = 1; i < templates[b.templateId()]._points.size(); i++)
		{
			point p = templates[b.templateId()]._points[i];
			p.first += b.bias().first;
			p.second += b.bias().second;
			vertexes.push_back(p);
		}
	}
	else if (b.ori() == OriType::MY) {
		for (int i = 1; i < templates[b.templateId()]._points.size(); i++)
		{
			point p = templates[b.templateId()]._points[i];
			p.first = b.bias().first - p.first;
			p.second += b.bias().second;
			vertexes.push_back(p);
		}
	}
	else if (b.ori() == OriType::MX) {
		for (int i = 1; i < templates[b.templateId()]._points.size(); i++)
		{
			point p = templates[b.templateId()]._points[i];
			p.first += b.bias().first;
			p.second = b.bias().second - p.second;
			vertexes.push_back(p);
		}
	}
	else {//R180
		for (int i = 1; i < templates[b.templateId()]._points.size(); i++)
		{
			point p = templates[b.templateId()]._points[i];
			p.first = b.bias().first - p.first;
			p.second = b.bias().second - p.second;
			vertexes.push_back(p);
		}
	}
	
	for (int i = 0; i < vertexes.size(); i++)
	{
		//Line L(0);//pid and id seem useless
		//L.setId(Lines.size());//
		point p1 = vertexes[i];
		point p2 = vertexes[(i + 1) % vertexes.size()];
		Line L(Lines.size(),p1, p2);
		L.setpId(b.id());
		L.setType(LineType::EDGE);
		b.setEdges(L);
		Lines.push_back(L);//Synchronize to Lines
	}
}

void Route::parser(std::string file)
{
	std::ifstream ifid(file);
	std::string line;
	std::string delims(" \n\r<>{}\t");
	getline(ifid,line);
	std::string tmp;
	tmp=tokenizer(line,delims); //<CONSTRAINTS>
	tmp=tokenizer(line,delims);
	w=stod(tmp);
	tmp=tokenizer(line,delims);
	h=stod(tmp);
	tmp=tokenizer(line,delims);
	d=stod(tmp);
	getline(ifid,line); //<BLOCK>
	getline(ifid,line);
 
	while(line.substr(0, 7)!="<BLOCK>")
	{
		BlockTemplate blocktem;
		tmp = tokenizer(line, delims);
		blocktem._name=tmp;
		while ((tmp = tokenizer(line, delims)) != blocktem._name) 
		{
			point p;
			p.first = stod(tmp);
			tmp = tokenizer(line, delims);
			p.second = stod(tmp);
			blocktem._points.push_back(p);
		}
		templates.push_back(blocktem);
		getline(ifid, line);
	}
	getline(ifid, line);	//<INSTANCE>
	getline(ifid, line);
	int blockid=0;
	while (line.substr(0, 10) != "<INSTANCE>")
	{
		Block block1(blockid);
		tmp = tokenizer(line, delims);
		block1.setName(tmp);
		tmp = tokenizer(line, delims);
		int i = 0;
		for (; i < templates.size(); i++)
			if (tmp == templates[i]._name) {
				templates[i]._instsId.push_back(blockid);
				blockid++;
				break;
			}
		block1.setTemplateId(i);
		tmp = tokenizer(line, delims);
		point p;
		p.first = stod(tmp);
		tmp = tokenizer(line, delims);
		p.second = stod(tmp);
		block1.setBias(p);
		tmp = tokenizer(line, delims);
		block1.setOri(tmp);
		blockedgeform(block1);
		blocks.push_back(block1);
		getline(ifid, line);
	}

	getline(ifid, line);	//<NODE>
	getline(ifid, line);
	while (line.substr(0, 6) != "<NODE>")
	{
		Node n;
		tmp = tokenizer(line, delims);
		n._name = tmp;
		tmp = tokenizer(line, delims);
		n._p.first = stod(tmp);
		tmp = tokenizer(line, delims);
		n._p.second = stod(tmp);
		nodes.push_back(n);


		getline(ifid, line);
	}
	getline(ifid, line);	//<FLY_LINE>
	getline(ifid, line);
	while (line.substr(0, 10) != "<FLY_LINE>")
	{
		tmp = tokenizer(line, delims);
		point p1;
		for(int i=0;i<nodes.size();i++)
			if (tmp == nodes[i]._name) 
			{
				p1 = nodes[i]._p;
				break;
			}
		std::string name1 = tmp;
		tmp = tokenizer(line, delims);
		point p2;
		for (int i = 0; i < nodes.size(); i++)
			if (tmp == nodes[i]._name)
			{
				p2 = nodes[i]._p;
				break;
			}
		FlyLine fl(flylines.size(), p1, p2,name1,tmp);
		flylines.push_back(fl);
		getline(ifid, line);
	}
	ifid.close();
}

void Route::output(std::string file)
{
	std::ofstream fout(file);
	for (FlyLine flyline : flylines)
  {
    flyline.sortLines();
	  flyline.printLines(fout);
  }
	fout.close();
}

bool Route::pointLegal(Line& line)
{
  for(auto &node : nodes) {
    point p = node._p, p1 = line.p1(), p2 = line.p2();
    if(pointEqual(p, p1) || pointEqual(p, p2))
      continue;
      
    // horizontal
    if(line.isHorizonal()) {
      if(std::abs(p.second - p1.second) > d) {
        continue;
      }else {
        if((p.first-p1.first)*(p.first - p2.first) <= 0)
        return false;
      }
    }
    // vertical
    else {
      if(std::abs(p.first - p1.first) > d) {
        continue;
      }else {
        if((p.second-p1.second)*(p.second - p2.second) <= 0)
        return false;
      }
    }
  }
  return true;
}

bool Route::pointLegal(Line& line, point pt1, point pt2)
{
  for(auto &node : nodes) {
    point p = node._p;
    
    if(pointEqual(p, pt1) || pointEqual(p, pt2))
      continue;
    
    point p1 = line.p1(), p2 = line.p2();
    // horizontal
    if(line.isHorizonal()) {
      if(std::abs(p.second - p1.second) > d) {
        continue;
      }else {
        if((p.first-p1.first)*(p.first - p2.first) < 0 || approxEqual(p.first, p1.first) || approxEqual(p.first, p2.first))
          return false;        
      }
    }
    // vertical
    else {
      if(std::abs(p.first - p1.first) > d) {
        continue;
      }else {
        if((p.second-p1.second)*(p.second - p2.second) < 0 || approxEqual(p.second, p1.second) || approxEqual(p.second, p2.second))
          return false;
      }
    }
  }
  return true;
}