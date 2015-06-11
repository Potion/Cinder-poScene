#pragma once

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