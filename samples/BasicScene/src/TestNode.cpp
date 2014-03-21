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

void TestNode::draw() {
    ci::gl::color(fillColor);
    ci::gl::drawSolidRect(ci::Rectf(0,0, 50,50));
}