#include "poShape.h"

#include "NodeTestRoot.h"

NodeTestRootRef NodeTestRoot::create() {
    NodeTestRootRef root(new NodeTestRoot());
    root->setup();
    return root;
}

void NodeTestRoot::setup() {
    setAlpha(0.5f);
    setAlignment(po::Alignment::CENTER_CENTER);
    setPosition(ci::app::getWindowWidth()/2, ci::app::getWindowHeight()/2);
    //setDrawBoundsEnabled(true);
    
    float nSquares = 10;
    for(int i=0; i<nSquares; i++) {
        po::NodeContainerRef thisGroup = po::NodeContainer::create();
        
        for(int j=0; j<nSquares; j++) {
            po::ShapeRef square = po::Shape::createRect(50, 50);
            square->alignment(po::Alignment::CENTER_CENTER)
            .fillColor(255,255,255)
            .rotation((360.f/nSquares) * j)
            .position(100,100)
            .alpha(0.5f)
            .setDrawBoundsEnabled(true);
            
            ci::app::timeline().apply(&square->getRotationAnim(), square->getRotation() - 360.f, 1.f * j).loop();
            
            
            ci::app::console() << square->isVisible() << std::endl;
            thisGroup->addChild(square);
            thisGroup->setVisibilityEnabled(false);
            ci::app::console() << square->isVisible() << std::endl;
            thisGroup->setVisibilityEnabled(true);
            
        }
        
        ci::app::timeline().apply(&thisGroup->getRotationAnim(), 360.f, 1.f * i).loop();
        
        thisGroup->position(i * 50, i * 50)
        //.alignment(po::Alignment::CENTER_CENTER)
        .setDrawBoundsEnabled(true);
        
        addChild(thisGroup);
    }
    
    
    
    ci::app::timeline().apply(&getRotationAnim(), 360.f, 5.f).loop();
    
}

void NodeTestRoot::update() {
    //ci::app::console() << getChildren()[0]->isVisible() << std::endl;
}
