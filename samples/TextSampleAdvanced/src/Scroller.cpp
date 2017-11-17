#include "Scroller.h"

#include "cinder/app/App.h"

ScrollerRef Scroller::create()
{
	ScrollerRef ref = std::shared_ptr<Scroller>(new Scroller());
	ref->setup();
	return ref;
}

Scroller::Scroller()
: mIsPressed(false)
, mStartPos(ci::vec2())
, mEndPos(ci::vec2())
, mInitialPos(ci::vec2())
{
}

Scroller::~Scroller() 
{
}

void Scroller::setup()
{
	//	Create the scroll track
	mTrack = ShapeView::createRect(10, ci::app::getWindowHeight() - 40);
	mTrack->setFillColor(ci::Color(1, 1, 1));
	mTrack->setAlpha(0.2);
	addSubview(mTrack);
	
	//	Create the scroll thumb
	mThumb = ShapeView::createRect(10, 100);
	mThumb->setFillColor(ci::Color(1, 1, 1));
	mThumb->setAlpha(0.8);
	addSubview(mThumb);
	
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
		mStartPos = getSuperview()->windowToLocal(event.getWindowPos());
		mEndPos = getSuperview()->windowToLocal(event.getWindowPos());
		
		//	Highlight the scroll thumb
		mThumb->setAlpha(1.0);
	}
}

void Scroller::onMouseDrag(po::scene::MouseEvent &event)
{
	if (mIsPressed) {
		//	Get the end window position relative to the parent
		mEndPos = getSuperview()->windowToLocal(event.getWindowPos());
		
		//	Calculate the new position
		ci::vec2 newPosition = ci::vec2(mInitialPos.x, mInitialPos.y + (mEndPos.y - mStartPos.y));
		
		if (newPosition.y < 0) {
			newPosition = ci::vec2(mInitialPos.x, 0);
		}
		
		if (newPosition.y > (mTrack->getHeight() - mThumb->getHeight())) {
			newPosition = ci::vec2(mInitialPos.x, mTrack->getHeight() - mThumb->getHeight());
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
