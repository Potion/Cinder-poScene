#include "BoundsSample.h"
#include "poShape.h"

using namespace po::scene;

float hueIncrement = 0.05f;

float hue = 0.5f;
float nodeContainerHue = hue + hueIncrement;
float nodeHue = nodeContainerHue + hueIncrement;

float brightness = 0.65f;
float selectedBrightness = 1.0f;

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
    .alignment(ci::TextBox::Alignment::RIGHT)
    .font(ci::Font("Arial", 11));
    
    // Scene BG
    mBG = Shape::createRect(1, 1);
    mBG->setFillColor(ci::Color(ci::CM_HSV, hue, saturation, brightness))
    .setParentShouldIgnoreInBounds(true);
    addChild(mBG);
    
    // Scene text box + window text box
    mWindowTextBox = TextBox::create();
    mWindowTextBox->setPosition(-250, 0)
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
    
    mNodeContainer->addChild(mNodeContainerBG)
    .addChild(mNodeContainerTextBox)
    .getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&BoundsSample::nodeMouseOver, this, std::placeholders::_1));
    
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
        .getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&BoundsSample::nodeMouseOver, this, std::placeholders::_1));
        mNodeContainer->addChild(s);
    }
    
    // Setup Scene (this class is the root node)
    setName("Scene (Scene Root Node)")
    .setPosition(ci::Vec2f(250, 50))
    .setDrawBounds(true)
    .setBoundsColor(boundsColor);
    
    ci::app::getWindow()->connectKeyDown(&BoundsSample::keyPressed, this);
}

void BoundsSample::update()
{
    // Set the BGs
    mBG->setPosition(getBounds().getUpperLeft())
    .setScale(getSize());
    
    mNodeContainerBG->setPosition(mNodeContainer->getBounds().getUpperLeft())
    .setScale(mNodeContainer->getSize());
    
    // Update the scene and node container text
    mInfoText.setText(getNodeInfo(shared_from_this()));
    mTextBox->setCiTextBox(mInfoText);
    
    
    mInfoText.setText(getNodeInfo(mNodeContainer));
    mNodeContainerTextBox->setCiTextBox(mInfoText);
    
    // Update window text
    std::stringstream ss;
    ss << "Window Mouse Position: " << ci::app::App::get()->getMousePos()-ci::app::getWindow()->getPos();
    
    
    if(mSelectedNode) {
        ss << "\n\n" << getNodeInfo(mSelectedNode)
        << "\n---------------------------------"
        << "\n To rotate press 'r'"
        << "\n To adjust alignment press 'a'";
    } else {
        ss << "\n\n Click a node to select\nand view/change attributes...";
    }
    
    mInfoText.text(ss.str())
    .alignment(ci::TextBox::Alignment::RIGHT);
    mWindowTextBox->setCiTextBox(mInfoText);
    
    mInfoText.alignment(ci::TextBox::Alignment::LEFT);
}

void BoundsSample::nodeMouseOver(po::scene::MouseEvent &event)
{
    
    if(mSelectedNode)
    {
        if(mSelectedNode == mNodeContainer) {
            mNodeContainerBG->setFillColor(ci::Color(ci::CM_HSV, nodeContainerHue, saturation, brightness));
        } else {
            mSelectedNode->setFillColor(ci::Color(ci::CM_HSV, nodeHue, saturation, brightness));
        }
    }
    
    mSelectedNode       = event.getSource();
    if(mSelectedNode == mNodeContainer) {
        mNodeContainerBG->setFillColor(ci::Color(ci::CM_HSV, nodeContainerHue, saturation, selectedBrightness));
    } else {
        mSelectedNode->setFillColor(ci::Color(ci::CM_HSV, nodeHue, saturation, selectedBrightness));
    }
}

void BoundsSample::keyPressed(ci::app::KeyEvent &key)
{
    if(!mSelectedNode)
        return;
    
    switch (key.getChar()) {
        case 'r':
            mSelectedNode->setRotation(mSelectedNode->getRotation() + 30.0f);
            break;
        case 'a': {
            int curAlignment = static_cast<int>(mSelectedNode->getAlignment());
            curAlignment += 1;
            if(curAlignment > static_cast<int>(po::scene::Alignment::NONE)) {
                curAlignment = 0;
            }
            
            mSelectedNode->setAlignment(static_cast<po::scene::Alignment>(curAlignment));
        }
            break;
        default:
            break;
    }
    
    std::cout << getHeight() << std::endl;
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