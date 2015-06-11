#include "BoundsSample.h"
#include "poShape.h"

using namespace po::scene;

float hueIncrement = 0.05f;

float hue = 0.5f;
float nodeContainerHue = hue + hueIncrement;
float nodeHue = nodeContainerHue + hueIncrement;

float brightness = 0.65f;
float nodeSelectedBrightness = 1.0f;

float saturation = 0.5f;

ci::Color boundsColor(1.0f, 1.0f, 1.0f);

BoundsSampleRef BoundsSample::create()
{
    BoundsSampleRef ref(new BoundsSample());
    ref->setup();
    return ref;
}

void BoundsSample::setup()
{
    
    // Set up our info text
    mInfoText.size(200, ci::TextBox::GROW)
    .color(ci::Color(1, 1, 1))
    .font(ci::Font("Arial", 11));
    
    // Scene BG
    mBG = Shape::createRect(1, 1);
    mBG->setFillColor(ci::Color(ci::CM_HSV, hue, saturation, brightness));
    addChild(mBG);
    
    // Scene text box + window text box
    mWindowTextBox = TextBox::create();
    mWindowTextBox->setPosition(0, 400)
    .setParentShouldIgnoreInBounds(true);
    addChild(mWindowTextBox);
    
    mTextBox = TextBox::create();
    mTextBox->setPosition(5, 5)
    .setParentShouldIgnoreInBounds(true);
    addChild(mTextBox);
    
    // Node Container
    mNodeContainer = NodeContainer::create("Node Container");
    mNodeContainer->setPosition(100, 150)
    .setDrawBounds(true)
    .setBoundsColor(boundsColor)
    .setName("Node Container");
    
    mNodeContainerBG = Shape::createRect(1, 1);
    mNodeContainerBG->setFillColor(ci::Color(ci::CM_HSV, nodeContainerHue, saturation, brightness))
    .setParentShouldIgnoreInBounds(true);
    
    mNodeContainerTextBox = TextBox::create();
    
    mNodeContainerTextBox->setPosition(5, 5)
    .setParentShouldIgnoreInBounds(true);
    
    mNodeContainer->addChild(mNodeContainerBG);
    mNodeContainer->addChild(mNodeContainerTextBox);
    
    addChild(mNodeContainer);
    
    // Add some shapes
    int yPos = 175;
    for(int i=0; i<3; i++) {
        ShapeRef s = Shape::createRect(100, 100);
        s->setPosition(i * 125, yPos)
        .setName("Selected Node")
        .setDrawBounds(true)
        .setBoundsColor(boundsColor)
        .setFillColor(ci::Color(ci::CM_HSV, nodeHue, saturation, brightness))
        .setAlignment(Alignment::CENTER_CENTER)
        .getSignal(MouseEvent::Type::MOVE_INSIDE).connect(std::bind(&BoundsSample::nodeMouseOver, this, std::placeholders::_1));
        mNodeContainer->addChild(s);
    }
    
    // Setup Scene (this class is the root node)
    setName("Scene (Scene Root Node)")
    .setPosition(ci::Vec2f(ci::app::getWindowWidth()/2 - getWidth()/2, 50))
    .setDrawBounds(true)
    .setBoundsColor(boundsColor);
}

void BoundsSample::update()
{
    // Set the BGs
    mBG->setScale(getSize());
    
    mNodeContainerBG->setPosition(mNodeContainer->getBounds().getUpperLeft())
    .setScale(mNodeContainer->getSize());
    
    std::cout << mNodeContainer->getBounds() << std::endl;
    
    // Update the scene and node container text
    mInfoText.setText(getNodeInfo(shared_from_this()));
    mTextBox->setCiTextBox(mInfoText);
    
    mInfoText.setText(getNodeInfo(mNodeContainer));
    mNodeContainerTextBox->setCiTextBox(mInfoText);
    
    // Update window text
    std::stringstream ss;
    ss << "Window Mouse Position: " << ci::app::App::get()->getMousePos()-ci::app::getWindow()->getPos();
    
    
    if(mSelectedNode) {
        ss << "\n\n" << getNodeInfo(mSelectedNode);
    }
    
    mInfoText.text(ss.str());
    mWindowTextBox->setCiTextBox(mInfoText);
}

void BoundsSample::nodeMouseOver(po::scene::MouseEvent &event)
{
    if(mSelectedNode)
    {
        mSelectedNode->setFillColor(ci::Color(ci::CM_HSV, nodeHue, saturation, brightness));
    }
    
    mSelectedNode = event.getSource();
    mSelectedNode->setFillColor(ci::Color(ci::CM_HSV, nodeHue, saturation, nodeSelectedBrightness));
}


std::string BoundsSample::getNodeInfo(po::scene::NodeRef node)
{
    std::stringstream ss;
    ss << node->getName()
    << "\n---------------------------------"
    << "\nWindow Pos " << node->localToWindow(ci::Vec2f(0,0))
    << "\nScene Pos: " << node->localToScene(ci::Vec2f(0,0))
    << "\nPosition: " << node->getPosition()
    << "\nLocal Mouse Position: " << node->windowToLocal(ci::app::App::get()->getMousePos()-ci::app::getWindow()->getPos())
    << "\nBounds: " << node->getBounds()
    << "\nFrame: " << node->getFrame();
    
    return ss.str();
}