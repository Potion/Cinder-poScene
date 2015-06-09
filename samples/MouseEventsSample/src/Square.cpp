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
, mIsMouseDown(false)
{
}

Square::~Square() 
{
}

void Square::setup()
{
	setAlignment(po::scene::Alignment::CENTER_CENTER);
	
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
	
	//	Connect to mouse events
	getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&Square::onMouseEvent, this, std::placeholders::_1));
	getSignal(MouseEvent::UP_INSIDE).connect(std::bind(&Square::onMouseEvent, this, std::placeholders::_1));
	getSignal(MouseEvent::UP).connect(std::bind(&Square::onMouseEvent, this, std::placeholders::_1));
}

void Square::onMouseEvent(po::scene::MouseEvent &event)
{
	switch (event.getType()) {
		case po::scene::MouseEvent::DOWN_INSIDE:
			mIsMouseDown = true;
			setSelected(true);
			break;
		case po::scene::MouseEvent::UP_INSIDE:
			if (mIsMouseDown) doRotate();
			break;
		case po::scene::MouseEvent::UP:
			setSelected(false);
			break;
		default:
			break;
	}
}

void Square::setSelected(bool isSelected)
{
	//	Fade the selected state in and out
	if (isSelected) {
		ci::app::timeline().apply(&mSelected->getAlphaAnim(), 1.0f, 0.2f);
	} else {
		ci::app::timeline().apply(&mSelected->getAlphaAnim(), 0.0f, 0.5f)
		.finishFn(std::bind(&Square::reset, this));
	}
}

void Square::doRotate()
{
	ci::app::timeline().apply(&getRotationAnim(), 360.f, 0.5f)
	.finishFn(std::bind(&Square::reset, this));
}

void Square::reset()
{
	setRotation(0.f);
	mIsMouseDown = false;
}