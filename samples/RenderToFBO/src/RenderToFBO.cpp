#include "poShape.h"

#include "RenderToFBO.h"

RenderToFBORef RenderToFBO::create() {
    RenderToFBORef node(new RenderToFBO());
    node->setup();
    return node;
}


void RenderToFBO::setup() {
    float width         = 10000;
    float height        = 5000;
    float circleSize    = 200;
    for(int i=0; i<width/circleSize; i++) {
        po::ShapeRef thisCircle = po::Shape::createCircle(circleSize);
        thisCircle->setPosition(ci::Vec2f(i*circleSize, 0));
        thisCircle->fillColor(255,255,255);
        addChild(thisCircle);
    }
    
    ci::app::console() << getBounds().getWidth()    << std::endl;
    ci::app::console() << getBounds().getHeight()   << std::endl;
    ci::app::console() << getNumChildren()          << std::endl;
    
    ci::app::timeline().apply(&getRotationAnim(), 360.f, 1.f).loop();
}
