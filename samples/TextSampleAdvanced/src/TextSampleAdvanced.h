#pragma once

#include "poScene/View.h"

#include "TextComponent.h"
#include "Scroller.h"

using namespace po::scene;

class TextSampleAdvanced;
typedef std::shared_ptr<TextSampleAdvanced> TextSampleAdvancedRef;

class TextSampleAdvanced
: public po::scene::View
{
public:
    static TextSampleAdvancedRef create();
    
    void setup();
    
protected:
	TextSampleAdvanced();
	
	void update();

private:
	//	Reference to the text component
	TextComponentRef mTextComponent;
	
	//	Reference to the scroller
	ScrollerRef mScroller;

};
