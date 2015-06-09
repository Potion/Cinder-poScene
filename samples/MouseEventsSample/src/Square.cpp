#include "Square.h"

SquareRef Square::create()
{
	SquareRef ref = std::shared_ptr<Square>(new Square());
	ref->setup();
	return ref;
}

Square::Square() 
{
}

Square::~Square() 
{
}

void Square::setup()
{
	mShape = Shape::createRect(100, 100);
	mShape->setFillColor(255.f/255, 123.f/255, 172.f/255);
	addChild(mShape);
}