//
//  TestNode.cpp
//  BasicScene
//
//  Created by Stephen Varga on 3/18/14.
//
//

#include "TestNode.h"

TestNodeRef TestNode::create() {
    return std::shared_ptr<TestNode>(new TestNode());
}

TestNode::TestNode()
{
    setInteractionEnabled(true);
}

void TestNode::draw() {
    ci::gl::color(ci::Color(255,0,0));
    ci::gl::drawSolidRect(ci::Rectf(0,0, 50,50));
}

//Events

void TestNode::mouseDown(po::MouseEvent event)
{
    std::cout << "Test node mouse down!" << std::endl;
}

void TestNode::mouseUp(po::MouseEvent event)
{
    std::cout << "Test node Mouse Up event!" << std::endl;
}