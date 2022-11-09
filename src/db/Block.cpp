 #include "Block.h"

 ///@brief Judge whether a point is inside a block.If the point is inside the block, return true.
    bool Block::judgeInside(point p)
{
    int v = 0;//Number of intersections with vertical lines
    int k = 0;
    double top= _edges[0].p2().second;
    double bottom = _edges[0].p1().second;
    for (int i = 1; i < _edges.size(); i++)
    {
        if(_edges[i].p2().second > top)
            top = _edges[i].p2().second;
        if (_edges[i].p1().second < bottom)
            bottom = _edges[i].p1().second;

    }

    for (int i = 0; i < _edges.size(); i++)
    {
        if (!_edges[i].isHorizonal())//vertical
        {
            if ((_edges[i].p1().second < p.second || approxEqual(_edges[i].p1().second, p.second)) && (_edges[i].p2().second > p.second || approxEqual(_edges[i].p2().second, p.second)))
            {
                if (approxEqual(p.first, _edges[i].p1().first))
                    return true;    //point on a vertical line;
                if (_edges[i].p2().first > p.first)
                    v++;
            }                
        }
        else
        {
            if (approxEqual(p.second, _edges[i].p1().second))
            {
                if ((_edges[i].p1().first < p.first || approxEqual(_edges[i].p1().first, p.first)) && (_edges[i].p2().first > p.first || approxEqual(_edges[i].p2().first, p.first)))
                    return true; //point on a horizonal line
                if (p.first < _edges[i].p1().first && p.second<top && p.second>bottom)
                    v--;
            }
        }

    }
    if (v % 2 == 1)
        return true;
    else
        return false;
}

    /*! @brief Judge whether a line intersects the block, and retrun intersection points. If no intersections, the vector size is 0.*/
    std::vector<point> Block::judgeLineIntersect(Line l)
    {
        std::vector<point> intersections;
        bool horizon=l.isHorizonal();
        for(int i=0;i<_edges.size();i++)
        {
            if(horizon)
            {
                if(!_edges[i].isHorizonal())
                    if(_edges[i].p1().second<l.p1().second&&_edges[i].p2().second>l.p1().second&&_edges[i].p1().first<l.p2().first&&_edges[i].p1().first>l.p1().first)
                    {
                        point a(_edges[i].p1().first,l.p1().second);
                        intersections.push_back(a);
                    }
                        
            }
            else
            {
                if(_edges[i].isHorizonal())
                    if(_edges[i].p1().first<l.p1().first&&_edges[i].p2().first>l.p1().second&&_edges[i].p1().second<l.p2().second&&_edges[i].p1().first>l.p1().second)
                    {
                        point a(l.p1().first,_edges[i].p1().second);
                        intersections.push_back(a);
                    }
            }
        }
        return intersections; 
    }
