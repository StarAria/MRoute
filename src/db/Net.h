/*! @file db/Net.h
    @brief Net class.
    @date 2022/11/30
*/

#ifndef __NET_H__
#define __NET_H__

#include "./Type.h"
#include "./FlyLine.h"
#include "./Node.h"
#include <vector>

/*! @class Net
    @brief Net class
*/

class Net
{
private:
    int                     _id;
    std::vector<int>        _nodesId;
    std::vector<int>        _flyLinesId;
};

#endif