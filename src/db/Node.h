/*! @file db/Node.h
    @brief Node header file.
    @date 2022/11/30
*/

#ifndef __NODE_H__
#define __NODE_H__

#include "./Type.h"
#include <string>

/*! @class Node
    @brief Node class for single node instance.
*/

class Node
{
private:
    int             _id;
    int             _blockId;
    int             _netId;
    point           _loc;
    std::string     _name;
public:
    Node(/* args */);
    
};



#endif