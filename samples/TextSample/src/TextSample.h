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
	
private:
	TextSample();
	
	TextComponentRef mTextComponent;
	ScrollerRef mScroller;
	
	void onMouseDrag(MouseEvent &event);

};
