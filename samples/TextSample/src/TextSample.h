#pragma once

#include "poNodeContainer.h"
#include "TextComponent.h"
#include "Scroller.h"

using namespace po::scene;

class TextSample;
typedef std::shared_ptr<TextSample> TextSampleRef;

class TextSample
: public po::scene::NodeContainer
{
public:
    static TextSampleRef create();
    
    void setup();
    
protected:
	void update();
	
private:
	TextSample();
	
	//	Reference to the text component
	TextComponentRef mTextComponent;
	
	//	Reference to the scroller
	ScrollerRef mScroller;

};
