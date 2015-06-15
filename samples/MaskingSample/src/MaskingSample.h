#pragma once

#include "poNodeContainer.h"
#include "poShape.h"
#include "poImage.h"

using namespace po::scene;

class MaskingSample;
typedef std::shared_ptr<MaskingSample> MaskingSampleRef;

class MaskingSample
: public po::scene::NodeContainer
{
public:
    static MaskingSampleRef create();
    
    void setup();
    
protected:
	MaskingSample();

private:
	//	Image
	ImageRef mImage;
	
	//	Mask shape
	ShapeRef mMask;
	
	//	Mouse event handlers
	void onMouseMove(MouseEvent &event);
	
	void keyUp(ci::app::KeyEvent &event);

};
