#include "Square.h"

SquareRef Square::create()
{
	SquareRef ref = std::shared_ptr<Square>(new Square());
	ref->setup();
	return ref;
}

Square::Square()
: mDownColor(255.f/255, 147.f/255, 30.f/255)
, mUpColor(255.f/255, 123.f/255, 172.f/255)
{
}

Square::~Square() 
{
}

void Square::setup()
{
	mShape = Shape::createRect(100, 100);
	mShape->setFillColor(mUpColor);
	addChild(mShape);
}

void Square::setIsDown(bool isDown)
{
	if (isDown) {
		mShape->setFillColor(mDownColor);
	} else {
		mShape->setFillColor(mUpColor);
	}
}