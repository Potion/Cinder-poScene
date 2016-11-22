#include "Indicator.h"

#include "cinder/app/App.h"

using namespace po::scene;

IndicatorRef Indicator::create(std::string name, ci::Color color)
{
    IndicatorRef ref = std::shared_ptr<Indicator>(new Indicator());
    ref->setup(name, color);
    return ref;
}

Indicator::Indicator()
{
}

Indicator::~Indicator()
{
}

void Indicator::setup(std::string name, ci::Color color)
{
    mColor = color;
    
    //	Create and add the highlight shape
    //	Set alpha to 0 so we can animate it later
    mHighlight = ShapeView::createRect(100, 20);
    mHighlight->setFillColor(mColor);
    addChild(mHighlight);
    mHighlight->setAlpha(0);
    
    //	Create a text box
    ci::TextBox textbox = ci::TextBox();
    textbox.text(name);
    textbox.color(ci::Color(1, 1, 1));
    textbox.size(90, 10);
    textbox.setAlignment(ci::TextBox::Alignment::CENTER);
    
    //	Add it to the text node
    mTextBox = TextView::create(textbox);
    addChild(mTextBox);
    mTextBox->setPosition(5, 5);
}

void Indicator::showHighlighted()
{
    //	Fade the highlight in and out
    ci::app::timeline().apply(&mHighlight->getAlphaAnim(), 1.0f, 0.5f);
    ci::app::timeline().appendTo(&mHighlight->getAlphaAnim(), 0.0f, 0.5f);
}