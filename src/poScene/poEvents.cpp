//
//  poEvent.cpp
//  BasicScene
//
//  Created by Stephen Varga on 3/22/14.
//
//

#include "poEvents.h"

namespace po {

    #pragma mark - Mouse Event -
    MouseEvent::MouseEvent(MouseEvent::Type type, ci::Vec2f windowPos) {
        this->windowPos = pos;
        this->type = type;
    }
    
    void MouseEvent::getPos(){}
}