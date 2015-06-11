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
	mTextComponent = TextComponent::create();
	addChild(mTextComponent);
	mTextComponent->setPosition(20, 20);
	
	mScroller = Scroller::create();
	addChild(mScroller);
	mScroller->setPosition(mTextComponent->getWidth() + mTextComponent->getPosition().x + 10, mTextComponent->getPosition().y);
}

void TextSample::onMouseDrag(po::scene::MouseEvent &event)
{
	
}