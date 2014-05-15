#include "poShape.h"

#include "NodeTestRoot.h"

NodeTestRootRef NodeTestRoot::create() {
    NodeTestRootRef root(new NodeTestRoot());
    root->setup();
    return root;
}

void NodeTestRoot::setup() {
//    setAlpha(0.5f);
//    setAlignment(po::Alignment::CENTER_CENTER);
//    setPosition(ci::app::getWindowWidth()/2, ci::app::getWindowHeight()/2);
//    setDrawBoundsEnabled(true);
//    
//    float nSquares = 10;
//    for(int i=0; i<nSquares; i++) {
//        po::NodeContainerRef thisGroup = po::NodeContainer::create();
//        
//        for(int j=0; j<nSquares; j++) {
//            po::ShapeRef square = po::Shape::createRect(50, 50);
//            square->alignment(po::Alignment::CENTER_CENTER)
//            .fillColor(255,255,255)
//            .rotation((360.f/nSquares) * j)
//            .alpha(0.15f);
//            
//            ci::app::timeline().apply(&square->getRotationAnim(), square->getRotation() - 360.f, 1.f * j).loop();
//            ci::app::timeline().apply(&square->getPositionAnim(), ci::Vec2f(2.f * j, 0.f), 1.f * j).pingPong();
//            
//            square->getSignalMouseMoveInside().connect(std::bind(&NodeTestRoot::mouseMoveInside, this, std::_1));
//            
//            thisGroup->addChild(square);
//        }
//        
//        ci::app::timeline().apply(&thisGroup->getRotationAnim(), 360.f, 1.f * i).loop();
//        
//        thisGroup->alignment(po::Alignment::CENTER_CENTER)
//        .scale(0.5f + 0.25f * i, 0.5f + 0.25f * i);
//        
//        addChild(thisGroup);
//    }
//    
//    ci::app::timeline().apply(&getRotationAnim(), 360.f, 5.f).loop();
    
    setPosition(50,50);
    a = po::Shape::createRect(50, 50);
    b = po::Shape::createRect(50, 50);
    b->setPosition(-50,-50);
    b->setFillColor(ci::Color(1,0,1));
    
    addChild(a);
    addChild(b);
    
    setDrawBoundsEnabled(true);
}

void NodeTestRoot::update() {
    //ci::app::console() << getChildren()[0]->isVisible() << std::endl;
}

void NodeTestRoot::mouseMoveInside(po::MouseEvent &event)
{
    event.getSource()->fillColor(ci::Color(255,0,0));
    event.setShouldPropagate(true);
}
