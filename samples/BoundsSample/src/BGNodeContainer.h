#pragma once

/*	Created by Stephen Varga on 6/11/15.
 *	Copyright 2015 __MyCompanyName__. All rights reserved.
 */

#include "poNodeContainer.h"
#include "poShape.h"
#include "poTextBox.h"

class BGNodeContainer;
typedef std::shared_ptr<BGNodeContainer> BGNodeContainerRef;

class BGNodeContainer
: public po::scene::NodeContainer
{
public:
    static BGNodeContainerRef create(std::string name, ci::Color bgFillColor);
	virtual ~BGNodeContainer();
    
    void setBGFillColor(ci::Color color) {mBGColor = color; };

protected:
    BGNodeContainer();
    
    virtual void setup(std::string name, ci::Color bgFillColor);
    virtual void update();
    void draw();

private:
    ci::Color mBGColor;
    po::scene::TextBoxRef mTextBox;
    ci::TextBox mText;
};