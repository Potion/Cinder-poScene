//
//  TestNode.h
//  BasicScene
//
//  Created by Stephen Varga on 3/18/14.
//
//

#pragma once

#include "poNode.h"

class TestNode;
typedef std::shared_ptr<TestNode> TestNodeRef;

class TestNode
:   public po::Node
{
public:
    static TestNodeRef create();
    void draw();
    
    void mouseDown(po::MouseEvent event);
    void mouseUp(po::MouseEvent event);
protected:
    TestNode();
};
