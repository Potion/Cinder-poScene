#include "BoundsSample.h"
#include "BGNodeContainer.h"
#include "poShape.h"

using namespace po::scene;

BoundsSampleRef BoundsSample::create() 
{
    BoundsSampleRef ref(new BoundsSample());
    ref->setup();
    return ref;
}


void BoundsSample::setup() 
{
    BGNodeContainer::setup("Scene Root Node", ci::Color(0.0f, 0.2f, 0.2f));
        
    BGNodeContainerRef bgContainer = BGNodeContainer::create("Node Container", ci::Color(0.0f, 0.3f, 0.3f));
    bgContainer->setPosition(100, 100);
    addChild(bgContainer);
    
    // Add some shapes
    int yPos = 100;
    for(int i=0; i<3; i++) {
        ShapeRef s = Shape::createRect(100, 100);
        s->setPosition(i * 125, yPos);
        s->getSignal(MouseEvent::Type::MOVE_INSIDE).connect(std::bind(&BoundsSample::nodeMouseOver, this, std::placeholders::_1));
        bgContainer->addChild(s);
    }
    
    mText.size(400, ci::TextBox::GROW)
    .color(ci::Color(1, 1, 1))
    .font(ci::Font("Helvetica", 10));
    
    mTextBox = TextBox::create(mText);
    mTextBox->setPosition(0, 350);
    addChild(mTextBox);
    
    
}

void BoundsSample::update()
{
    BGNodeContainer::update();
    
    std::stringstream ss;
    ss << "Window Mouse Position: " << ci::app::App::get()->getMousePos()-ci::app::getWindow()->getPos();
    
    
    if(mSelectedNode) {
        ss << "\n---------------------------------\nSelected Node"
        << "\n\nWindow Pos " << mSelectedNode->localToWindow(ci::Vec2f(0,0))
        << "\nScene Pos: " << mSelectedNode->localToScene(ci::Vec2f(0,0))
        << "\nPosition: " << mSelectedNode->getPosition()
        << "\nLocal Mouse Position: " << mSelectedNode->windowToLocal(ci::app::App::get()->getMousePos()-ci::app::getWindow()->getPos());
    }
    
    mText.text(ss.str());
    mTextBox->setCiTextBox(mText);
}

void BoundsSample::nodeMouseOver(po::scene::MouseEvent &event)
{
    if(mSelectedNode)
    {
        mSelectedNode->setFillColor(ci::Color(1.0f, 1.0f, 1.0f));
    }
    
    mSelectedNode = event.getSource();
    mSelectedNode->setFillColor(ci::Color(1.0f, 0.0f, 0.0f));
}