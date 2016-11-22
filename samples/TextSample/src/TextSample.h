#pragma once

#include "poScene/View.h"
#include "poScene/TextView.h"
#include "poScene/ShapeView.h"

using namespace po::scene;

class TextSample;
typedef std::shared_ptr<TextSample> TextSampleRef;

class TextSample
: public po::scene::View
{
public:
    static TextSampleRef create();
    
    void setup();
    
protected:
	
private:
	TextSample();
	
	//	Scaling text box
	TextViewRef mScalingText;
	
	//	Scale text up and down in a loop
	void animateScale();

};
