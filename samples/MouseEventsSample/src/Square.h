#pragma once

#include "poNodeContainer.h"
#include "poShape.h"

using namespace po::scene;

class Square;
typedef std::shared_ptr<Square> SquareRef;

class Square
: public po::scene::NodeContainer
{
public:
	static SquareRef create();
	virtual ~Square();

	virtual void setup();
	
	void setIsDown(bool isDown);

protected:
	Square();

private:
	ShapeRef mShape;
	ci::Color mDownColor;
	ci::Color mUpColor;
	
};