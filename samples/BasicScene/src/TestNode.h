//
//  TestNode.h
//  BasicScene
//
//  Created by Stephen Varga on 3/18/14.
//
//

#pragma once

#include "poNodeContainer.h"
#include "poShape.h"

class TestNode;
typedef std::shared_ptr<TestNode> TestNodeRef;

class TestNode
:   public po::NodeContainer
{
public:
    static TestNodeRef create();
    
    void setup();
    void draw();
    
    void mouseDown(po::MouseEvent event);
    void mouseUp(po::MouseEvent event);
    void mouseDownInside(po::MouseEvent event);
    void mouseMoveInside(po::MouseEvent event);
    void mouseUpInside(po::MouseEvent event);
protected:
    TestNode();
private:
    po::ShapeRef rect;
};
