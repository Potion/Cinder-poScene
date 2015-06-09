#include "Square.h"

SquareRef Square::create()
{
	SquareRef ref = std::shared_ptr<Square>(new Square());
	ref->setup();
	return ref;
}

Square::Square()
: mSelectedColor(255.f/255, 147.f/255, 30.f/255)
, mActiveColor(255.f/255, 123.f/255, 172.f/255)
{
}

Square::~Square() 
{
}

void Square::setup()
{
	//	Create and add a shape for the active state
	mActive = Shape::createRect(100, 100);
	mActive->setFillColor(mActiveColor);
	addChild(mActive);
	
	//	Create and add a shape for the selected state
	//	Set the alpha to 0 so we can animate it
	mSelected = Shape::createRect(100, 100);
	mSelected->setFillColor(mSelectedColor);
	addChild(mSelected);
	mSelected->setAlpha(0.f);
}

void Square::showSelected()
{
	//	Fade the selected state in and out
	ci::app::timeline().apply(&mSelected->getAlphaAnim(), 1.0f, 0.2f);
	ci::app::timeline().appendTo(&mSelected->getAlphaAnim(), 0.0f, 0.5f);
}