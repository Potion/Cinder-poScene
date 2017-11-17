#include "Indicator.h"
#include "cinder/Text.h"

using namespace po::scene;

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
    //	Set alpha to 0 so we can highlight it later
    mHighlight = ShapeView::createRect(135, 20);
    mHighlight->setFillColor(mColor);
    addSubview(mHighlight);
    mHighlight->setAlpha(0);
    
    //	Create a text box
    ci::TextBox textbox;
    textbox.text(name)
    .color(ci::Color(1, 1, 1))
    .size(125, 10);
    
    //	Add it to the text node
    mTextBox = TextView::create(textbox);
    addSubview(mTextBox);
    mTextBox->setPosition(5, 5);
}

void Indicator::showHighlighted()
{
    mHighlight->setAlpha(1.f);
}

void Indicator::hideHighlighted()
{
    mHighlight->setAlpha(0.f);
}