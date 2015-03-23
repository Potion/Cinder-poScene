#pragma once

#include "poNodeContainer.h"

class RenderToFBO;
typedef std::shared_ptr<RenderToFBO> RenderToFBORef;

class RenderToFBO
: public po::NodeContainer
{
public:
    static RenderToFBORef create();
    
    void setup();
    
    void circlePressed(po::MouseEvent &event);
    
    #pragma message "Should we declare all events for this template?"
protected:
    po::NodeContainerRef mContainer;
    int mWidth, mHeight, mCircleSize;
};
