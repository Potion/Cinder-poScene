#include "poShape.h"

#include "NodeTestRoot.h"

NodeTestRootRef NodeTestRoot::create() {
    NodeTestRootRef root(new NodeTestRoot());
    root->setup();
    return root;
}

void NodeTestRoot::setup() {
    setAlpha(0.5f);
    
    float nSquares = 10;
    for(int i=0; i<nSquares; i++) {
        po::ShapeRef square = po::Shape::createRect(50, 50);
        square->alignment(po::Alignment::CENTER_CENTER)
        .position(50, 50)
        .fillColor(255,255,255)
        .rotation((360.f/nSquares) * i)
        .alpha(0.5f);
        
        addChild(square);
    }
}

void NodeTestRoot::update() {
    ci::app::console() << getChildren()[0]->isVisible() << std::endl;
}
