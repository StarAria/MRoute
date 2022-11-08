/*! @class Block
    @brief Bloc instance.
*/

#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "./Type.h"
#include "./Line.h"
#include <vector>

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
    void setTemplateId(int id) {_templateId=id;}
    void setName(std::string name) {_name=name;}
    void setBias(point bias) {_bias=bias;}
    void setOri(std::string ori) 
    {
        if (ori == "R0") _ori = OriType::R0;
        if (ori == "MX") _ori = OriType::MX;
        if (ori == "MY") _ori = OriType::MY;
        if (ori == "R180") _ori = OriType::R180;
    }

    ///@brief set edges of the block one by one
    void setEdges(Line edge) {_edges.push_back(edge);}

    //functions to get data members
    int id() const {return _id;}
    int templateId() const {return _templateId;}
    std::string name() const {return _name;}
    point bias() const {return _bias;}
    OriType ori() const {return _ori;}
    std::vector<Line> edges() {return _edges;} //can return & to get faster
    
    ///@brief Judge whether a point is inside a block.If the point is inside the block, return true.
    bool judgeInside(point p);

    /*! @brief Judge whether a line intersects the block, and retrun intersection points. 
        If no intersections, the vector size is 0.*/
    std::vector<point> judgeLineIntersect(Line l);

};

#endif
