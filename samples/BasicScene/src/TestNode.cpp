//
//  TestNode.cpp
//  BasicScene
//
//  Created by Stephen Varga on 3/18/14.
//
//

#include "TestNode.h"

TestNodeRef TestNode::create() {
    return TestNodeRef(new TestNode());
}

TestNode::TestNode()
{
    setInteractionEnabled(true);
}

void TestNode::setup()
{
    //position.set(50,50);
    
    rect = po::Shape::createRect(150, 150);
    rect->setInteractionEnabled(true);
    rect->position.set(50,50);
    rect->rotation.set(0, 0, 45);
    addChild(rect);
    
//    addEvent(po::MouseEvent::Type::DOWN_INSIDE, shared_from_this());
//    addEvent(po::MouseEvent::Type::MOVE_INSIDE, shared_from_this());
//    addEvent(po::MouseEvent::Type::UP_INSIDE, shared_from_this());
    
    addEvent(po::MouseEvent::Type::DOWN_INSIDE, rect);
    addEvent(po::MouseEvent::Type::MOVE_INSIDE, rect);
    addEvent(po::MouseEvent::Type::UP_INSIDE,   rect);
}

void TestNode::draw() {
    ci::gl::color(ci::Color(255,0,0));
    ci::gl::drawSolidRect(ci::Rectf(0,0, 50,50));
}

//Events
void TestNode::mouseDown(po::MouseEvent event)
{
}

void TestNode::mouseDownInside(po::MouseEvent event)
{
    rect->fillColor.set(255,0,0);
}

void TestNode::mouseMoveInside(po::MouseEvent event)
{
    std::cout << "Test node mouse move inside!" << std::endl;
}

void TestNode::mouseUpInside(po::MouseEvent event)
{
    std::cout << "Test node mouse up inside!" << std::endl;
}

void TestNode::mouseUp(po::MouseEvent event)
{
    //std::cout << "Test node Mouse Up event!" << std::endl;
    rect->fillColor.set(255,255,255);
}