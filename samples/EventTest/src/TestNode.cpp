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
    
    for(int i=0; i<NUM_ROWS; i++) {
		for(int j=0; j<NUM_COLS; j++) {
            po::ShapeRef r = po::Shape::createRect(SIZE, SIZE);
            r->setFillColor(ci::Color(1,0,1));
            r->setAlpha(1.f);
            //r->setInteractionEnabled(true);
            
            //Set Position in grid
			float xPos = j * (SIZE + SPACING);
			float yPos = i * (SIZE + SPACING);
			r->setPosition(xPos, yPos);
            r->setAlignment(po::Alignment::CENTER_CENTER);
            r->setDrawBounds(true);
            
            r->getSignalMouseDownInside().connect(std::bind(&TestNode::mouseDownInside, this, std::_1));
            r->getSignalMouseMoveInside().connect(std::bind(&TestNode::mouseMoveInside, this, std::_1));
//            boost::shared_ptr<po::Node> a(this);
//            boost::weak_ptr<po::Node> n(a);
//            r->getSignalMouseMoveInside().connect(po::MouseEventSignal::slot_type(&TestNode::mouseMoveInside, this, std::_1).track(a));
            
            r->getSignalTouchesBeganInside().connect(std::bind(&TestNode::touchesBeganInside, this, std::_1));
            r->getSignalTouchesMovedInside().connect(std::bind(&TestNode::touchesMovedInside, this, std::_1));
            r->getSignalTouchesEndedInside().connect(std::bind(&TestNode::touchesEndedInside, this, std::_1));
            
            addChild(r);
        }
    }
    
    setPosition((ci::app::getWindowWidth()/2)-(getWidth()/2) - getBounds().x1, ci::app::getWindowHeight()/2-getHeight()/2 - getBounds().y1);
}

void TestNode::squareFinishedTweening(po::ShapeRef square) {
    square->setFillColor(0,255,0);
    std::cout << "Finished!" << std::endl;
}

//Events
void TestNode::myMouseHandler(po::MouseEvent &event)
{
    std::cout <<  "My Mouse Handler!!!" << std::endl;
}

void TestNode::mouseDown(po::MouseEvent& event)
{
//    std::cout << "Left down: " << event.isLeftDown() << std::endl;
//    std::cout << "Right down: " << event.isRightDown() << std::endl;
//    std::cout << "Center down: " << event.isMiddleDown() << std::endl;
    

    
}

void TestNode::mouseDrag(po::MouseEvent& event)
{
}


void TestNode::mouseMove(po::MouseEvent& event)
{
}

void TestNode::mouseDownInside(po::MouseEvent& event)
{
    po::ShapeRef thisRect = std::static_pointer_cast<po::Shape>(event.getSource());
    
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
    
    ci::Vec2f globalPos(thisRect->localToGlobal(event.getPos()));
    
    std::cout << "Local Pos: "      << event.getPos()                       << std::endl;
    std::cout << "Global Pos"       << globalPos                            << std::endl;
    std::cout << "Converted Pos"    << thisRect->globalToLocal(globalPos)   << std::endl;
    
    //thisRect->disconnectMouseDownInside(this);
    //event.setShouldPropagate(true);
    
    //thisRect->emitMouseUpInside(event);
}

void TestNode::mouseMoveInside(po::MouseEvent& event)
{
//    po::ShapeRef thisRect = std::static_pointer_cast<po::Shape>(event.getSource());
    //    thisRect->fillColor.set(0,0,255);
//    std::cout << "Local Pos: "  << event.getPos() << std::endl;
//    std::cout << "Scene Pos: "  << event.getScenePos() << std::endl;
//    std::cout << "Global Pos: " << event.getWindowPos() << std::endl;
//
//    std::cout << "Window position: " << event.getWindowPos() << std::endl;
//    std::cout << "Node position: " << (event.getPos())  << std::endl;
//    std::cout << "localToGlobal: "  << event.getSource()->localToGlobal(event.getPos()) << std::endl;
}

void TestNode::mouseDragInside(po::MouseEvent& event)
{
    std::cout << "Mouse drag inside!" << std::endl;
}

void TestNode::mouseUpInside(po::MouseEvent& event)
{
    std::cout << "Mouse up inside!" << std::endl;
}

void TestNode::mouseUp(po::MouseEvent& event)
{
    //std::cout << "Test node Mouse Up event!" << std::endl;
    //rect->fillColor.set(255,255,255);
}

void TestNode::touchesBeganInside(po::TouchEvent &event)
{
    std::cout << "Touches Began Inside!" << std::endl;
}

void TestNode::touchesMovedInside(po::TouchEvent &event)
{
    std::cout << "Touches Moved Inside!" << std::endl;
}

void TestNode::touchesEndedInside(po::TouchEvent &event)
{
    std::cout << "Touches ended inside!" << std::endl;
}

void TestNode::keyDown(po::KeyEvent &event)
{
    std::cout << "Key down!" << std::endl;
}

void TestNode::keyUp(po::KeyEvent &event)
{
    std::cout << "Key Up!" << std::endl;
}