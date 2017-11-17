#include "Indicator.h"

#include "cinder/app/App.h"
#include "cinder/Text.h"

IndicatorRef Indicator::create(std::string name)
{
	IndicatorRef ref = std::shared_ptr<Indicator>(new Indicator());
	ref->setup(name);
	return ref;
}

Indicator::Indicator()
: mColor(ci::Color(255.f/255, 147.f/255, 30.f/255))
{
}

Indicator::~Indicator() 
{
}

void Indicator::setup(std::string name)
{
	//	Create and add the highlight shape
	//	Set alpha to 0 so we can animate it later
	mHighlight = ShapeView::createRect(100, 20);
	mHighlight->setFillColor(mColor);
	mHighlight->setAlpha(0);
	
	//	Create a text box
	ci::TextBox textbox = ci::TextBox();
	textbox.text(name);
	textbox.color(ci::Color(1, 1, 1));
	textbox.size(100, 10);
	
	//	Add it to the text node
	mTextBox = TextView::create(textbox);
	mTextBox->setPosition(5, 5);

	addSubview(mHighlight);
	addSubview(mTextBox);
}

void Indicator::showHighlighted()
{
	//	Fade the highlight in and out
	ci::app::timeline().apply(&mHighlight->getAlphaAnim(), 1.0f, 0.2f);
	ci::app::timeline().appendTo(&mHighlight->getAlphaAnim(), 0.0f, 0.2f);
}