#include "Indicator.h"
#include "cinder/Text.h"

IndicatorRef Indicator::create(std::string name)
{
	IndicatorRef ref = std::shared_ptr<Indicator>(new Indicator());
	ref->setup(name);
	return ref;
}

Indicator::Indicator()
: mHighlightedColor(ci::Color(255.f/255, 226.f/255, 139.f/255))
, mUnhighlightedColor(ci::Color(0, 0, 0))
{
}

Indicator::~Indicator() 
{
}

void Indicator::setup(std::string name)
{
	mHighlight = Shape::createRect(100, 30);
	mHighlight->setFillColor(mUnhighlightedColor);
	addChild(mHighlight);
	
	mTextBox = TextBox::create();
	mTextBox->getCiTextBox()->text(name);
	mTextBox->getCiTextBox()->color(ci::Color(1, 1, 1));
	mTextBox->getCiTextBox()->size(100, 30);
	mTextBox->render();
	addChild(mTextBox);
}

void Indicator::setIsHighlighted(bool isHighlighted)
{
	if (isHighlighted) {
		mHighlight->setFillColor(mHighlightedColor);
	} else {
		mHighlight->setFillColor(mUnhighlightedColor);
	}
}