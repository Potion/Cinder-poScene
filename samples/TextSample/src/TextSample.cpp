#include "TextSample.h"
#include "cinder/ImageIo.h"
TextSampleRef TextSample::create() 
{
    TextSampleRef node(new TextSample());
    node->setup();
    return node;
}

TextSample::TextSample()
{
}

void TextSample::setup() 
{
	std::string text = "Have secret plans intrigued by the shower hack up furballs, for kick up litter so loves cheeseburgers, for poop on grasses scamper. Leave hair everywhere stand in front of the computer screen.";
	
	//	Create the ci textbox
	ci::TextBox ciTextBox = ci::TextBox();
	ciTextBox.size(200, 100);
	ciTextBox.color(ci::Color(1, 1, 1));
	ciTextBox.text(text);
	ciTextBox.alignment(ci::TextBox::Alignment::CENTER);
	ciTextBox.font(ci::Font("Arial", 13));
	
	//	Create a text box that rotates
	TextBoxRef rotatingText = TextBox::create(ciTextBox);
	rotatingText->setAlignment(po::scene::Alignment::CENTER_CENTER)
	.setPosition(150, 150);
	addChild(rotatingText);
	ci::app::timeline().apply(&rotatingText->getRotationAnim(), 360.f, 10.0f).loop();
	
	//	Create a text box that scales up and down
	mScalingText = TextBox::create(ciTextBox);
	mScalingText->setAlignment(po::scene::Alignment::CENTER_CENTER)
	.setPosition(150, 350);
	addChild(mScalingText);
	animateScale();
	
	//	Create a text box that scrolls across the screen
	TextBoxRef movingText = TextBox::create(ciTextBox);
	movingText->setPosition(400, ci::app::getWindowHeight());
	addChild(movingText);
	ci::app::timeline().apply(&movingText->getPositionAnim(), ci::Vec2f(movingText->getPosition().x, -movingText->getHeight()), 10.f)
	.easeFn(ci::EaseInSine())
	.loop();
	
	//	Load in the mask texture and create a mask shape
	ci::gl::TextureRef texture = ci::gl::Texture::create(ci::loadImage(ci::app::loadAsset("mask.jpg")));
	ShapeRef mask = Shape::create(texture);
	
	//	Create a container to hold the text box
	NodeContainerRef textNode = NodeContainer::create();
	addChild(textNode);
	textNode->setPosition(400, 100);
	
	//	Create a text box to be masked and add it to the container
	TextBoxRef maskedText = TextBox::create(ciTextBox);
	textNode->addChild(maskedText);
	
	//	Apply a mask to the container
	textNode->setMask(mask);
	
	//	Animate the mask from left to right
	ci::app::timeline().apply(&mask->getPositionAnim(), ci::Vec2f(-textNode->getWidth(), -textNode->getHeight()), ci::Vec2f(textNode->getWidth(), 0), 3.0f).loop();
}

//	Scale down and up
void TextSample::animateScale()
{
	ci::app::timeline().apply(&mScalingText->getScaleAnim(), ci::Vec2f(0.6f, 0.6f), 1.f);
	ci::app::timeline().appendTo(&mScalingText->getScaleAnim(), ci::Vec2f(1.2f, 1.2f), 1.f)
	.easeFn(ci::EaseInSine())
	.finishFn(std::bind(&TextSample::animateScale, this));
}