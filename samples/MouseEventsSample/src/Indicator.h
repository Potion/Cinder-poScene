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
	static IndicatorRef create(std::string name);
	virtual ~Indicator();

	virtual void setup(std::string name);
	
	void setIsHighlighted(bool isHighlighted);

protected:
	Indicator();

private:
	TextBoxRef mTextBox;
	ShapeRef mHighlight;
	ci::Color mHighlightedColor;
	ci::Color mUnhighlightedColor;
	
};