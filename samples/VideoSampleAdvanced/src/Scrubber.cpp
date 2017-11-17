#include "Scrubber.h"

using namespace po::scene;

ScrubberRef Scrubber::create()
{
    ScrubberRef ref (new Scrubber());
    ref->setup();
    return ref;
}

Scrubber::Scrubber()
: mIsPressed(false)
, mStartPos(glm::zero<ci::vec2>())
, mEndPos(glm::zero<ci::vec2>())
, mInitialPos(glm::zero<ci::vec2>())
{}

void Scrubber::setup()
{
    //  create and add the scrub track
    mTrack = ShapeView::createRect(640, 10);
    mTrack->setFillColor(1, 1, 1);
    mTrack->setAlpha(0.2f);
    mTrack->setName("track");
    addSubview(mTrack);
    
    //  create and add the scrub handle
    mHandle = ShapeView::createRect(50, 10);
    mHandle->setFillColor(1, 1, 1);
    mHandle->setAlpha(0.8f);
    mHandle->setName("handle");
    addSubview(mHandle);
    
    mFullLength = mTrack->getWidth() - mHandle->getWidth();
    
    //  connect the mouse events
    //getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&Scrubber::onMouseDown, this, std::placeholders::_1));
    mTrack->getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&Scrubber::onMouseDownTrack, this, std::placeholders::_1));
    mHandle->getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&Scrubber::onMouseDownHandle, this, std::placeholders::_1));
    
    getSignal(MouseEvent::DRAG).connect(std::bind(&Scrubber::onMouseDrag, this, std::placeholders::_1));
    getSignal(MouseEvent::UP_INSIDE).connect(std::bind(&Scrubber::onMouseUp, this, std::placeholders::_1));
    getSignal(MouseEvent::UP).connect(std::bind(&Scrubber::onMouseUp, this, std::placeholders::_1));
}

void Scrubber::setHandlePosition(float pct)
{
    mHandle->setPosition(mFullLength * pct, mInitialPos.y);
}

void Scrubber::onMouseDownHandle(MouseEvent &event)
{
    if (!mIsPressed) {
        mIsPressed = true;
        
        //	Store the initial positions
        mInitialPos = mHandle->getPosition();
        mStartPos = getSuperview()->windowToLocal(event.getWindowPos());
        mEndPos = getSuperview()->windowToLocal(event.getWindowPos());
        
        //	Highlight the scroll thumb
        mHandle->setAlpha(1.0);
    }
}

void Scrubber::onMouseDownTrack(MouseEvent &event)
{
    if (!mIsPressed) {
        mIsPressed = true;
        
        ci::vec2 clickPos = getSuperview()->windowToLocal(event.getWindowPos());
        
        float newXPos = clickPos.x - mHandle->getWidth() / 2;
        if (newXPos < 0) {
            newXPos = 0;
        } else if (newXPos > mTrack->getWidth() - mHandle->getWidth()) {
            newXPos = mTrack->getWidth() - mHandle->getWidth();
        }

        mHandle->setPosition(newXPos, 0);
        
        //  Determine percentage
        float pct = newXPos / mFullLength;
        mScrubberSignal.emit(pct);
        
        //	With repositioned hadle, store the initial positions
        mInitialPos = mHandle->getPosition();
        mStartPos = getSuperview()->windowToLocal(event.getWindowPos());
        mEndPos = getSuperview()->windowToLocal(event.getWindowPos());
        
        //	Highlight the scroll thumb
        mHandle->setAlpha(1.0);
    }
}

void Scrubber::onMouseDrag(MouseEvent &event)
{
    if (mIsPressed) {
        //	Get the end window position relative to the parent
        mEndPos = getSuperview()->windowToLocal(event.getWindowPos());
        
        //	Calculate the new position
        ci::vec2 newPosition = ci::vec2(mInitialPos.x + (mEndPos.x - mStartPos.x), mInitialPos.y);
        
        if (newPosition.x < 0) {
            newPosition = ci::vec2(0, mInitialPos.y);
        }
        
        if (newPosition.x > (mTrack->getWidth() - mHandle->getWidth())) {
            newPosition = ci::vec2(mInitialPos.x, mTrack->getHeight() - mHandle->getHeight());
            newPosition = ci::vec2(mTrack->getWidth() - mHandle->getWidth(), mInitialPos.y);
        }
        
        //	Set the new position
        mHandle->setPosition(newPosition);
        
        //  Determine percentage
        float pct = newPosition.x / mFullLength;
        mScrubberSignal.emit(pct);
    }
}

void Scrubber::onMouseUp(po::scene::MouseEvent &event)
{
    if (mIsPressed) {
        mIsPressed = false;
        
        //	Unhighlight
        mHandle->setAlpha(0.8);
    }
}
