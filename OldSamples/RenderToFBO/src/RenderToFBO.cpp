#include "poShape.h"

#include "RenderToFBO.h"
#include "poTextbox.h"

RenderToFBORef RenderToFBO::create() {
    RenderToFBORef node(new RenderToFBO());
    node->setup();
    return node;
}


void RenderToFBO::setup() {
    po::NodeContainerRef prevContainer = std::static_pointer_cast<po::NodeContainer>(shared_from_this());
    
    for(int i=0; i<5; i++) {
        po::NodeContainerRef c = po::NodeContainer::create();
        c->setDrawBounds(true);
        c->setPosition(ci::Vec2f(25.0f*i, 25.0f*i));
        po::ShapeRef rect = po::Shape::createRect(20, 20);
        c->addChild(rect);
        
        prevContainer->addChild(c);
        prevContainer = c;
    }
    
    mContainer = prevContainer;
    
    mWidth         = 600;
    mHeight        = 400;
    mCircleSize    = 50;
    
    for(int i=0; i< mWidth/mCircleSize; i++) {
        po::ShapeRef thisCircle = po::Shape::createCircle(mCircleSize);
        thisCircle->setPosition(ci::Vec2f(i * mCircleSize, 0));
        thisCircle->fillColor(255,255,255);
        //thisCircle->getSignalMouseDownInside().connect(std::bind(&RenderToFBO::circlePressed, this, std::placeholders::_1));
        mContainer->addChild(thisCircle);
    }
    
    setCacheToFboEnabled(true, ci::app::getWindowWidth(), ci::app::getWindowHeight());
    setPosition(ci::app::getWindowWidth()/2, ci::app::getWindowHeight()/2);
    //setAlignment(po::Alignment::CENTER_CENTER);
    mContainer->getSignalMouseDown().connect(std::bind(&RenderToFBO::circlePressed, this, std::placeholders::_1));

    
    //ci::app::timeline().apply(&container->getRotationAnim(), 360.f, 1.f).loop();
    
    
//    po::ShapeRef c = po::Shape::createCircle(50);
//    c->fillColor(255,0,0);
//    addChild(c);
//    
//    po::TextBoxRef t = po::TextBox::create();
//    t->setText("test");
//    t->setFont(ci::Font("Helvetica", 50));
//    t->render();
//    
//    addChild(t);
}

void RenderToFBO::circlePressed(po::MouseEvent &event)
{
    po::ShapeRef thisCircle = po::Shape::createCircle(mCircleSize);
    thisCircle->setAlignment(po::Alignment::CENTER_CENTER);
    thisCircle->setPosition(event.getPos());
    thisCircle->fillColor(255,0,255);
    //thisCircle->getSignalMouseDown().connect(std::bind(&RenderToFBO::circlePressed, this, std::placeholders::_1));
    mContainer->addChild(thisCircle);
    
    std::cout << event.getPos() << std::endl;
    
//    std::cout << event.getPos() << std::endl;
//    if(event.getSource())
    //event.getSource()->setFillColor(ci::Color(255, 0, 0));
}
