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
}