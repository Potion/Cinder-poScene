#pragma once

#include "poScene/View.h"
#include "poScene/TextView.h"
#include "poScene/ShapeView.h"

class Indicator;
typedef std::shared_ptr<Indicator> IndicatorRef;

class Indicator
: public po::scene::View
{
public:
    static IndicatorRef create(std::string name, ci::Color color);
    virtual ~Indicator();
    
    virtual void setup(std::string name, ci::Color color);
    
    //	Show highlight
    void showHighlighted();
    
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