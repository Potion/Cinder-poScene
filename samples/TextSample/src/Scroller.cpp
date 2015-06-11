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
, mInitialPos(ci::Vec2f::zero())
{
}

Scroller::~Scroller() 
{
}

void Scroller::setup()
{
	//	Create the scroll track
	mTrack = Shape::createRect(10, ci::app::getWindowHeight() - 40);
	mTrack->setFillColor(ci::Color(1, 1, 1));
	mTrack->setAlpha(0.2);
	addChild(mTrack);
	
	//	Create the scroll thumb
	mThumb = Shape::createRect(10, 100);
	mThumb->setFillColor(ci::Color(1, 1, 1));
	mThumb->setAlpha(0.8);
	addChild(mThumb);
	
	//	Connect the mouse events
	getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&Scroller::onMouseDown, this, std::placeholders::_1));
	getSignal(MouseEvent::DRAG).connect(std::bind(&Scroller::onMouseDrag, this, std::placeholders::_1));
	getSignal(MouseEvent::UP_INSIDE).connect(std::bind(&Scroller::onMouseUp, this, std::placeholders::_1));
	getSignal(MouseEvent::UP).connect(std::bind(&Scroller::onMouseUp, this, std::placeholders::_1));
}

void Scroller::onMouseDown(po::scene::MouseEvent &event)
{
	if (!mIsPressed) {
		mIsPressed = true;
		
		//	Store the initial positions
		mInitialPos = mThumb->getPosition();
		mStartPos = getParent()->windowToLocal(event.getWindowPos());
		mEndPos = getParent()->windowToLocal(event.getWindowPos());
		
		//	Highlight the scroll thumb
		mThumb->setAlpha(1.0);
	}
}

void Scroller::onMouseDrag(po::scene::MouseEvent &event)
{
	if (mIsPressed) {
		//	Get the end window position relative to the parent
		mEndPos = getParent()->windowToLocal(event.getWindowPos());
		
		//	Calculate the new position
		ci::Vec2f newPosition = ci::Vec2f(mInitialPos.x, mInitialPos.y + (mEndPos.y - mStartPos.y));
		
		if (newPosition.y < 0) {
			newPosition = ci::Vec2f(mInitialPos.x, 0);
		}
		
		if (newPosition.y > (mTrack->getHeight() - mThumb->getHeight())) {
			newPosition = ci::Vec2f(mInitialPos.x, mTrack->getHeight() - mThumb->getHeight());
		}
		
		//	Set the new position
		mThumb->setPosition(newPosition);
	}
}

void Scroller::onMouseUp(po::scene::MouseEvent &event)
{
	if (mIsPressed) {
		mIsPressed = false;
		
		//	Unhighlight
		mThumb->setAlpha(0.8);
	}
}