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
    setDrawBoundsEnabled(true);
    setPosition(50,50);
    
    for(int i=0; i<NUM_ROWS; i++) {
		for(int j=0; j<NUM_COLS; j++) {
            po::ShapeRef r = po::Shape::createRect(SIZE);
            //r->setInteractionEnabled(true);
            
            //Set Position in grid
			float xPos = j * (SIZE + SPACING);
			float yPos = i * (SIZE + SPACING);
			r->setPosition(xPos, yPos);
            //r->setAlignment(Alignment::CENTER_CENTER);
            r->setDrawBoundsEnabled(true);
            
            r->connectMouseDownInside(this);
            r->connectMouseMoveInside(this);
//            r->connectMouseDragInside(this);
//            r->connectMouseUpInside(this);
            
            //r->connectMouseDownInside(&TestNode::myMouseHandler, this);
            
            addChild(r);
        }
    }
    
    setPosition((ci::app::getWindowWidth()/2)-(getWidth()/2) - getBounds().x1, ci::app::getWindowHeight()/2-getHeight()/2 - getBounds().y1);
}

void TestNode::draw() {
//    ci::gl::color(ci::Color(255,0,0));
//    ci::gl::drawSolidRect(ci::Rectf(0,0, 50,50));
}

void TestNode::squareFinishedTweening(po::ShapeRef square) {
    square->fillColor.set(0,255,0);
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
    
    thisRect->fillColor.set(255,0,0);
    
    if(!thisRect->rotationAnim.isComplete()) {
        thisRect->rotationAnim.stop();
        squareFinishedTweening(thisRect);
    }
    else {
        thisRect->fillColor.set(255,0,0);
        
        if (round(thisRect->getRotation()) == 360.f) {
            thisRect->setRotation(0);
        }
        
        float animationTime = 4.f - (4.0f * (thisRect->getRotation()/360.f));
        ci::app::timeline().apply(&thisRect->rotationAnim, 360.f, animationTime)
                            .finishFn(std::bind( &TestNode::squareFinishedTweening,this, thisRect));
    }
    
    //thisRect->disconnectMouseDownInside(this);
    event.setShouldPropagate(true);
    
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
//    std::cout << "localToGlobal: "  << sceneToLocal(event.getPos()) << std::endl;
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

void TestNode::keyDown(po::KeyEvent &event)
{
    std::cout << "Key down!" << std::endl;
}

void TestNode::keyUp(po::KeyEvent &event)
{
    std::cout << "Key Up!" << std::endl;
}