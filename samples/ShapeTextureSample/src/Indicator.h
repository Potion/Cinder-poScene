#pragma once

#include "poScene/View.h"
#include "poScene/TextView.h"
#include "poScene/ShapeView.h"

using namespace po::scene;

class Indicator;
typedef std::shared_ptr<Indicator> IndicatorRef;

class Indicator
: public po::scene::View
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
    TextViewRef mTextBox;
    
    //	Highlight shape
    ShapeViewRef mHighlight;
    
    //	Highlight color
    ci::Color mColor;
    
};