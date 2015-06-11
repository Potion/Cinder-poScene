#pragma once

#include "poNodeContainer.h"
#include "poShape.h"

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
	ShapeRef mImage;
	
	//	Mask shape
	ShapeRef mMask;
	
	//	Mouse event handler
	void onMouseMove(MouseEvent &event);

};
