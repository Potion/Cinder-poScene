#pragma once

#include "poNodeContainer.h"
#include "Square.h"

using namespace po::scene;

class HierarchySample;
typedef std::shared_ptr<HierarchySample> HierarchySampleRef;

class HierarchySample
: public po::scene::NodeContainer
{
public:
    static HierarchySampleRef create();
    
    void setup();
    
protected:
	HierarchySample();
	
private:
	//	Container to hold all the squares
	NodeContainerRef mContainer;
	
	//	Reference to previous square
	SquareRef mPreviousSquare;

};
