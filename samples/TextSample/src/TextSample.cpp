#include "TextSample.h"

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
	//	Create the text component
	mTextComponent = TextComponent::create();
	addChild(mTextComponent);
	mTextComponent->setPosition(20, 20);
	
	//	Create the scroller and align it to the right of the text component
	mScroller = Scroller::create();
	addChild(mScroller);
	mScroller->setPosition(mTextComponent->getWidth() + mTextComponent->getPosition().x + 20, mTextComponent->getPosition().y);
}

void TextSample::update()
{
	//	Call baseclass update
	NodeContainer::update();
	
	//	Calculate the position of the text based on the scroll thumb postion
	float textPos = ci::lmap<float>(mScroller->getThumbPosition().y, 0, 340, -20, mTextComponent->getHeight() - ci::app::getWindowHeight() + 20);
	
	//	Animate text component to the new position
	ci::app::timeline().apply(&mTextComponent->getPositionAnim(), ci::Vec2f(mTextComponent->getPosition().x, -textPos), 0.2f);
}