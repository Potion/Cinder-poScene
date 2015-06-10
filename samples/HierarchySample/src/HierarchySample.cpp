#include "HierarchySample.h"

#include "poShape.h"

HierarchySampleRef HierarchySample::create() 
{
    HierarchySampleRef node(new HierarchySample());
    node->setup();
    return node;
}

HierarchySample::HierarchySample()
: mPreviousSquare(nullptr)
{
}

void HierarchySample::setup() 
{
	//	Number of squares
	int numSquares = 5;
	
	//	Maximum square size
	float maxSize = 300.f;
	
	//	Create the container
	mContainer = NodeContainer::create();
	addChild(mContainer);
//	mContainer->setDrawBounds(true);
	
	//	Add the first square to the container
	//	Add subsequent squares to the previous square
	for (int i = 0; i < numSquares; i++) {
		int size = maxSize - (i * (maxSize/numSquares));
		SquareRef square = Square::create(size);
		if (mPreviousSquare != nullptr) {
			mPreviousSquare->addChild(square);
		} else {
			mContainer->addChild(square);
		}
		mPreviousSquare = square;
	}
	
	//	Center everything
	mContainer->setPosition(ci::app::getWindowWidth() / 2, ci::app::getWindowHeight() / 2);
}
