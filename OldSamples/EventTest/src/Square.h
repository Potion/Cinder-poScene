//
//  Square.h
//  BasicScene
//
//  Created by Stephen Varga on 3/23/14.
//
//

#include "poShape.h"

class Square;
typedef std::shared_ptr<Square> SquareRef;

class Square {
public:
    static SquareRef create(float size);
    
    void setup();
    void mouseDownInside(po::MouseEvent event);
};