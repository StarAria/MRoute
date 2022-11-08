/*! @file main/Route.cpp
    @brief implementation of the Route class
*/

#include <fstream>
#include "Route.h"

bool Route::run()
{
  std::vector<Line> buffer;
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

bool Route::isLegal(Line& L1, Line& L2)
{
  auto L1p1 = L1.p1();
  auto L1p2 = L1.p2();
  auto L2p1 = L2.p1();
  auto L2p2 = L2.p2();

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
        if((L1p1.second-L2p1.second-d)*(L1p1.second-L2p2.second-d) <= 0 || (L1p2.second-L2p1.second)*(L1p2.second-L2p2.second-d) <= 0)
          return false;
        if((L2p1.second-L1p1.second-d)*(L2p1.second-L1p2.second-d) <= 0 || (L2p2.second-L1p1.second-d)*(L2p2.second-L1p2.second-d) <= 0) 
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
        if((L1p1.first-L2p1.first-d)*(L1p1.first-L2p2.first-d) <= 0 || (L1p2.first-L2p1.first-d)*(L1p2.first-L2p2.first-d) <= 0) 
          return false;
        if((L2p1.first-L1p1.first-d)*(L2p1.first-L1p2.first-d) <= 0 || (L2p2.first-L1p1.first-d)*(L2p2.first-L1p2.first-d) <= 0) 
          return false;
      }
    }
  }
  return true;
}

bool Route::syncAndCheck(std::vector<Line>& buffer)
{
  for(Line &L1 : buffer) {
    for(Line &L2 : Lines) {
      if(isLegal(L1, L2) == 0)
        return false;
    }
  }

  Lines.insert(Lines.end(), buffer.begin(), buffer.end());
  return true;
}

std::string tokenizer(std::string& line, const std::string& delims){
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
	
	for (int i = 0; i < vertexes.size() - 1; i++)
	{
		//Line L(0);//pid and id seem useless
		//L.setId(Lines.size());//
		point p1 = vertexes[i];
		point p2 = vertexes[i + 1];
		Line L(Lines.size(),p1, p2);
		L.setType(LineType::EDGE);
		b.setEdges(L);
		Lines.push_back(L);//Synchronize to Lines
	}
}

void Route::parser(std::string file){
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
 
	while(line!="<BLOCK>")
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
	while (line != "<INSTANCE>")
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
	while (line != "<NODE>")
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
	while (line != "<FLY_LINE>")
	{
		tmp = tokenizer(line, delims);
		point p1;
		for(int i=0;i<nodes.size();i++)
			if (tmp == nodes[i]._name) 
			{
				p1 = nodes[i]._p;
				break;
			}
		tmp = tokenizer(line, delims);
		point p2;
		for (int i = 0; i < nodes.size(); i++)
			if (tmp == nodes[i]._name)
			{
				p2 = nodes[i]._p;
				break;
			}
		FlyLine fl(flylines.size(),p1,p2);
		flylines.push_back(fl);
		getline(ifid, line);
	}
	ifid.close();
}
