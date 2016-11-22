#pragma once

#include "poScene/View.h"
#include "Square.h"

class HierarchySample;
typedef std::shared_ptr<HierarchySample> HierarchySampleRef;

class HierarchySample
: public po::scene::View
{
public:
    static HierarchySampleRef create();
    
    void setup();
    
protected:
	HierarchySample();
	
private:
	//	Container to hold all the squares
	po::scene::ViewRef mContainer;
	
	//	Reference to previous square
	SquareRef mPreviousSquare;

};
