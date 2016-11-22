#pragma once

#include "poScene/View.h"
#include "poScene/ShapeView.h"
#include "poScene/ImageView.h"

using namespace po::scene;

class MaskingSample;
typedef std::shared_ptr<MaskingSample> MaskingSampleRef;

class MaskingSample
: public po::scene::View
{
public:
    static MaskingSampleRef create();
    
    void setup();
    
protected:
	MaskingSample();

private:
	//	Image
	ImageViewRef mImage;
	
	//	Mask shape
	ShapeViewRef mMask;
	
	//	Mouse event handlers
	void onMouseMove(po::scene::MouseEvent &event);

};
