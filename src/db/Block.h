/*! @file db/Block.h
    @brief Block class.
    @date 2022/11/30
*/

#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "Type.h"
#include "Line.h"
#include <string>
#include <vector>

/*! @class Block
    @brief Bloc instance.
*/

class Block
{
private:
    int                     _id;
    int                     _templateId;
    std::string             _name;
    point                   _bias;
    std::vector<Line>       _edges;
    OriType                 _ori;
public:
    ///@brief constructor using id
    Block(int id):_id(id) {}

    ///@brief destructor
    ~Block() {}

    //can directly get from the input file
    void settemplateId(int id) {_templateId=id;}
    void setname(std::string name) {_name=name;}
    void setbias(point bias) {_bias=bias;}
    void setori(OriType ori) {_ori=ori;}

    ///@brief set edges of the block one by one
    void setedges(Line edge) {_edges.push_back(edge);}

    //functions to get data members
    int id() const {return _id;}
    int templateId() const {return _templateId;}
    std::string name() const {return _name;}
    point bias() const {return _bias;}
    OriType ori() const {return _ori;}
    std::vector<Line> edges() {return _edges;} //can return & to get faster
    
    ///@brief Judge whether a point is inside a block.If the point is inside the block, return true.
    bool judgeinside(point p);

    /*! @brief Judge whether a line intersects the block, and retrun intersection points. 
        If no intersections, the vector size is 0.*/
    std::vector<point> judgelineintersect(Line l);



    ///@brief Judge whether a point is inside a block.If the point is inside the block, return true.
    bool judgeinside(point p) 
    {
        int h=0,v=0;//Number of intersections with horizontal lines and vertical lines
        for(int i=0;i<_edges.size();i++)
        {
            if(_edges[i].ishorizonal())
            {
                if(_edges[i].p1().first<p.first&&_edges[i].p2().first>p.first)    //we can assume p1<p2; p1,p2 needs to return reference
                    h++;
            }
            else
                if(_edges[i].p1().second<p.second&&_edges[i].p2().second>p.second)
                    v++;
        }
        if(h%2==1&&v%2==1)
                return true;
            else
                return false;
    }

    /*! @brief Judge whether a line intersects the block, and retrun intersection points. If no intersections, the vector size is 0.*/
    std::vector<point> judgelineintersect(Line l)
    {
        std::vector<point> intersections;
        bool horizon=l.ishorizonal();
        for(int i=0;i<_edges.size();i++)
        {
            if(horizon)
            {
                if(!_edges[i].ishorizonal())
                    if(_edges[i].p1().second<l.p1().second&&_edges[i].p2().second>l.p1().second&&_edges[i].p1().first<l.p2().first&&_edges[i].p1().first>l.p1().first)
                    {
                        point a(_edges[i].p1().first,l.p1().second);
                        intersections.push_back(a);
                    }
                        
            }
            else
            {
                if(_edges[i].ishorizonal())
                    if(_edges[i].p1().first<l.p1().first&&_edges[i].p2().first>l.p1().second&&_edges[i].p1().second<l.p2().second&&_edges[i].p1().first>l.p1().second)
                    {
                        point a(l.p1().first,_edges[i].p1().second);
                        intersections.push_back(a);
                    }
            }
        }
        return intersections; 
    }


};

#endif
