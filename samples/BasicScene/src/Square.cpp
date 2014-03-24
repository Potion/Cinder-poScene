//
//  Square.cpp
//  BasicScene
//
//  Created by Stephen Varga on 3/23/14.
//
//

#include "Square.h"

SquareRef Square::create(float size)
{
    return po::Shape::create(size);
};

void Square::setup()
{
    
}

void Square::mouseDownInside