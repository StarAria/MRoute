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

};

#endif