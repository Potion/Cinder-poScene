#pragma once

#include "poNodeContainer.h"
#include "poTextBox.h"
#include "poShape.h"

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
	
	//	Scaling text box
	TextBoxRef mScalingText;
	
	//	Scale text up and down in a loop
	void animateScale();

};
