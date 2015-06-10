#pragma once

/*	Created by bruce on 6/9/15.
 *	Copyright 2015 __MyCompanyName__. All rights reserved.
 */

#include "poNodeContainer.h"
#include "poTextBox.h"
#include "poShape.h"

using namespace po::scene;

class Indicator;
typedef std::shared_ptr<Indicator> IndicatorRef;

class Indicator
: public po::scene::NodeContainer
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
    TextBoxRef mTextBox;
    
    //	Highlight shape
    ShapeRef mHighlight;
    
    //	Highlight color
    ci::Color mColor;
    
};