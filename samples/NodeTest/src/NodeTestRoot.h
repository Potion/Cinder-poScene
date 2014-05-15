#pragma once

#include "poNodeContainer.h"
#include "poShape.h"

class NodeTestRoot;
typedef std::shared_ptr<NodeTestRoot> NodeTestRootRef;

class NodeTestRoot
: public po::NodeContainer
{
public:
    static NodeTestRootRef create();
    
    void setup();
    void update();
    
    void mouseMoveInside(po::MouseEvent &event);
    
    #pragma message "Should we declare all events for this template?"
protected:
    po::ShapeRef a,b;
};
