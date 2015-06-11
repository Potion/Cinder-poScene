#include "BGNodeContainer.h"
#include "poScene.h"

using namespace po::scene;

BGNodeContainerRef BGNodeContainer::create(std::string name, ci::Color bgFillColor)
{
	BGNodeContainerRef ref = std::shared_ptr<BGNodeContainer>(new BGNodeContainer());
	ref->setup(name, bgFillColor);
	return ref;
}

BGNodeContainer::BGNodeContainer()
{
}

BGNodeContainer::~BGNodeContainer() 
{
}

void BGNodeContainer::setup(std::string name, ci::Color bgColor)
{
    setPosition(50, 50);
    setName(name);
    mBGColor = bgColor;
    
    
    mText.size(100, ci::TextBox::GROW)
    .color(ci::Color(1, 1, 1))
    .font(ci::Font("Helvetica", 10));
    
    mTextBox = TextBox::create();
    mTextBox->setPosition(5.0f, 5.0f);
    addChild(mTextBox);
}

void BGNodeContainer::update()
{
    std::stringstream ss;
    ss << getName()
        << "\n\nWindow Pos " << localToWindow(ci::Vec2f(0,0))
        << "\nScene Pos: " << localToScene(ci::Vec2f(0,0))
        << "\nPosition: " << getPosition()
        << "\nLocal Mouse Position: " << windowToLocal(ci::app::App::get()->getMousePos()-ci::app::getWindow()->getPos());
    mText.text(ss.str());
    mTextBox->setCiTextBox(mText);
}

void BGNodeContainer::draw()
{
    ci::gl::color(mBGColor);
    ci::gl::drawSolidRect(getBounds());
    
    NodeContainer::draw();
}