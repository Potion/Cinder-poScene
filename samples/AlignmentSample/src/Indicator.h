#pragma once

#include "poScene/ViewContainer.h"
#include "poScene/TextView.h"
#include "poScene/ShapeView.h"

class Indicator;
typedef std::shared_ptr<Indicator> IndicatorRef;

class Indicator
: public po::scene::ViewContainer
{
public:
    static IndicatorRef create(std::string name);
    virtual ~Indicator();
    
    virtual void setup(std::string name);
    
    //	Show highlight
    void showHighlighted();
    
    //  Hide highlight
    void hideHighlighted();
    
protected:
    Indicator();
    
private:
    //	Text box with event name
    po::scene::TextViewRef mTextBox;
    
    //	Highlight shape
    po::scene::ShapeViewRef mHighlight;
    
    //	Highlight color
    ci::Color mColor;
    
};