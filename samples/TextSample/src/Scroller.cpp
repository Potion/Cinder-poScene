#include "Scroller.h"

ScrollerRef Scroller::create()
{
	ScrollerRef ref = std::shared_ptr<Scroller>(new Scroller());
	ref->setup();
	return ref;
}

Scroller::Scroller()
: mIsPressed(false)
, mStartPos(ci::Vec2f::zero())
, mEndPos(ci::Vec2f::zero())
{
}

Scroller::~Scroller() 
{
}

void Scroller::setup()
{
	mThumb = Shape::createRect(20, 100);
	mThumb->setFillColor(ci::Color(1, 1, 1));
	mThumb->setAlpha(0.8);
	addChild(mThumb);
	
	getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&Scroller::onMouseDown, this, std::placeholders::_1));
	getSignal(MouseEvent::DRAG).connect(std::bind(&Scroller::onMouseDrag, this, std::placeholders::_1));
	getSignal(MouseEvent::UP_INSIDE).connect(std::bind(&Scroller::onMouseUp, this, std::placeholders::_1));
	getSignal(MouseEvent::UP).connect(std::bind(&Scroller::onMouseUp, this, std::placeholders::_1));
}

void Scroller::onMouseDown(po::scene::MouseEvent &event)
{
	if (!mIsPressed) {
		mIsPressed = true;
		mStartPos = getParent()->windowToLocal(event.getWindowPos());
		mEndPos = getParent()->windowToLocal(event.getWindowPos());
		ci::app::console() << "onMouseDown: " << getPosition() << ", " << mStartPos << ", " << mEndPos << std::endl;
	}
}

void Scroller::onMouseDrag(po::scene::MouseEvent &event)
{
	if (mIsPressed) {
		mEndPos = getParent()->windowToLocal(event.getWindowPos());
		ci::Vec2f newPosition = ci::Vec2f(getPosition().x, mEndPos.y - mStartPos.y);
		ci::app::console() << newPosition << std::endl;
		setPosition(newPosition);
		ci::app::console() << "onMouseDrag: " << getPosition() << ", " << mStartPos << ", " << mEndPos << std::endl;
	}
}

void Scroller::onMouseUp(po::scene::MouseEvent &event)
{
	if (mIsPressed) {
		mIsPressed = false;
		ci::app::console() << "onMouseUp: " << getPosition() << ", " << mStartPos << ", " << mEndPos << std::endl;
	}
}