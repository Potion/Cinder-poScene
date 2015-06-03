//
//  TestNode.cpp
//  BasicScene
//
//  Created by Stephen Varga on 3/18/14.
//
//
#include "cinder/Timeline.h"

#include "TestNode.h"

const int TestNode::NUM_ROWS		= 3;
const int TestNode::NUM_COLS		= 5;
const int TestNode::SIZE			= 70;
const int TestNode::SPACING         = 10;

using namespace po::scene;
using namespace std::placeholders;

TestNodeRef TestNode::create() {
    TestNodeRef t = TestNodeRef(new TestNode());
    t->setup();
    return t;
}

void TestNode::setup()
{
    
    ci::gl::enableAlphaBlending();
    setDrawBounds(true);
    setPosition(50,50);
    setRotation(45);
    
    //Add global events
    getSignal(MouseEvent::Type::DOWN).connect(std::bind(&TestNode::mouseDown,   this, std::placeholders::_1));
    getSignal(MouseEvent::Type::MOVE).connect(std::bind(&TestNode::mouseMove,   this, std::placeholders::_1));
    getSignal(MouseEvent::Type::DRAG).connect(std::bind(&TestNode::mouseDrag,   this, std::placeholders::_1));
	getSignal(MouseEvent::Type::UP).connect(std::bind(&TestNode::mouseUp,       this, std::placeholders::_1));
    
    for(int i=0; i<NUM_ROWS; i++) {
		for(int j=0; j<NUM_COLS; j++) {
            ShapeRef r = Shape::createRect(SIZE, SIZE);
            r->setFillColor(ci::Color(1,0,1));
            r->setAlpha(1.f);
            //r->setInteractionEnabled(true);
            
            //Set Position in grid
			float xPos = j * (SIZE + SPACING);
			float yPos = i * (SIZE + SPACING);
			r->setPosition(xPos, yPos);
            r->setAlignment(Alignment::CENTER_CENTER);
            r->setDrawBounds(true);
            
			r->getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&TestNode::mouseDownInside, this, std::placeholders::_1));
			r->getSignal(MouseEvent::Type::MOVE_INSIDE).connect(std::bind(&TestNode::mouseMoveInside, this, std::placeholders::_1));
//            boost::shared_ptr<Node> a(this);
//            boost::weak_ptr<Node> n(a);
//            r->getSignalMouseMoveInside().connect(MouseEventSignal::slot_type(&TestNode::mouseMoveInside, this, std::_1).track(a));
            
			r->getSignal(TouchEvent::Type::BEGAN_INSIDE).connect(std::bind(&TestNode::touchesBeganInside, this, std::placeholders::_1));
			r->getSignal(TouchEvent::Type::MOVED_INSIDE).connect(std::bind(&TestNode::touchesMovedInside, this, std::placeholders::_1));
			r->getSignal(TouchEvent::Type::ENDED_INSIDE).connect(std::bind(&TestNode::touchesEndedInside, this, std::placeholders::_1));
			
            addChild(r);
        }
    }
    
    setPosition((ci::app::getWindowWidth()/2)-(getWidth()/2) - getBounds().x1, ci::app::getWindowHeight()/2-getHeight()/2 - getBounds().y1);
}

void TestNode::squareFinishedTweening(ShapeRef square) {
    square->setFillColor(0,255,0);
    std::cout << "Finished!" << std::endl;
}

//Events
void TestNode::myMouseHandler(MouseEvent &event)
{
    std::cout <<  "My Mouse Handler!!!" << std::endl;
}

void TestNode::mouseDown(MouseEvent& event)
{
//    std::cout << "Left down: " << event.isLeftDown() << std::endl;
//    std::cout << "Right down: " << event.isRightDown() << std::endl;
//    std::cout << "Center down: " << event.isMiddleDown() << std::endl;
    
    ci::app::console() << "Mouse down!" << std::endl;
    
}

void TestNode::mouseDrag(MouseEvent& event)
{
    ci::app::console() << "Mouse drag!" << std::endl;
}


void TestNode::mouseMove(MouseEvent& event)
{
    //ci::app::console() << "Mouse move!" << std::endl;
}

void TestNode::mouseUp(MouseEvent& event)
{
    ci::app::console() << "Mouse up!" << std::endl;
}


void TestNode::mouseDownInside(MouseEvent& event)
{
    ShapeRef thisRect = std::static_pointer_cast<Shape>(event.getSource());
    
    thisRect->setFillColor(255,0,0);
    
    if(!thisRect->getRotationAnim().isComplete()) {
        thisRect->getRotationAnim().stop();
        squareFinishedTweening(thisRect);
    }
    else {
        thisRect->setFillColor(255,0,0);
        
        if (round(thisRect->getRotation()) == 360.f) {
            thisRect->setRotation(0);
        }
        
        float animationTime = 4.f - (4.0f * (thisRect->getRotation()/360.f));
        ci::app::timeline().apply(&thisRect->getRotationAnim(), 360.f, animationTime)
                            .finishFn(std::bind( &TestNode::squareFinishedTweening,this, thisRect));
    }
    
    ci::Vec2f globalPos(thisRect->localToWindow(event.getWindowPos()));
    
    std::cout << "Local Pos: "      << event.getLocalPos()                  << std::endl;
    std::cout << "Global Pos"       << globalPos                            << std::endl;
    std::cout << "Converted Pos"    << thisRect->windowToLocal(globalPos)   << std::endl;
    
    //thisRect->disconnectMouseDownInside(this);
    //event.setShouldPropagate(true);
    
    //thisRect->emitMouseUpInside(event);
}

void TestNode::mouseMoveInside(MouseEvent& event)
{
//    ShapeRef thisRect = std::static_pointer_cast<Shape>(event.getSource());
    //    thisRect->fillColor.set(0,0,255);
//    std::cout << "Local Pos: "  << event.getPos() << std::endl;
//    std::cout << "Scene Pos: "  << event.getScenePos() << std::endl;
//    std::cout << "Global Pos: " << event.getWindowPos() << std::endl;
//
//    std::cout << "Window position: " << event.getWindowPos() << std::endl;
//    std::cout << "Node position: " << (event.getPos())  << std::endl;
//    std::cout << "localToGlobal: "  << event.getSource()->localToGlobal(event.getPos()) << std::endl;
}

void TestNode::mouseDragInside(MouseEvent& event)
{
    std::cout << "Mouse drag inside!" << std::endl;
}

void TestNode::mouseUpInside(MouseEvent& event)
{
    std::cout << "Mouse up inside!" << std::endl;
}
void TestNode::touchesBeganInside(TouchEvent &event)
{
    std::cout << "Touches Began Inside!" << std::endl;
}

void TestNode::touchesMovedInside(TouchEvent &event)
{
    std::cout << "Touches Moved Inside!" << std::endl;
}

void TestNode::touchesEndedInside(TouchEvent &event)
{
    std::cout << "Touches ended inside!" << std::endl;
}