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
, mStartPos(ci::Vec2f::zero())
, mEndPos(ci::Vec2f::zero())
, mInitialPos(ci::Vec2f::zero())
{}

void Scrubber::setup()
{
    //  create and add the scrub track
    mTrack = Shape::createRect(640, 10);
    mTrack->setFillColor(1, 1, 1);
    mTrack->setAlpha(0.2f);
    addChild(mTrack);
    
    //  create and add the scrub handle
    mHandle = Shape::createRect(50, 10);
    mHandle->setFillColor(1, 1, 1);
    mHandle->setAlpha(0.8f);
    addChild(mHandle);
    
    mFullLength = mTrack->getWidth() - mHandle->getWidth();
    
    //  connect the mouse events
    getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&Scrubber::onMouseDown, this, std::placeholders::_1));
    getSignal(MouseEvent::DRAG).connect(std::bind(&Scrubber::onMouseDrag, this, std::placeholders::_1));
    getSignal(MouseEvent::UP_INSIDE).connect(std::bind(&Scrubber::onMouseUp, this, std::placeholders::_1));
    getSignal(MouseEvent::UP).connect(std::bind(&Scrubber::onMouseUp, this, std::placeholders::_1));
}

void Scrubber::setHandlePosition(float pct)
{
    std::cout << "Scrubber::setHandlePosition: full length: "<< mFullLength << ", pct: " << pct << std::endl;
    mHandle->setPosition(mFullLength * pct, mInitialPos.y);
}

void Scrubber::onMouseDown(MouseEvent &event)
{
    if (!mIsPressed) {
        mIsPressed = true;
        
        //	Store the initial positions
        mInitialPos = mHandle->getPosition();
        mStartPos = getParent()->windowToLocal(event.getWindowPos());
        mEndPos = getParent()->windowToLocal(event.getWindowPos());
        
        //	Highlight the scroll thumb
        mHandle->setAlpha(1.0);
    }

}

void Scrubber::onMouseDrag(MouseEvent &event)
{
    if (mIsPressed) {
        //	Get the end window position relative to the parent
        mEndPos = getParent()->windowToLocal(event.getWindowPos());
        
        //	Calculate the new position
        ci::Vec2f newPosition = ci::Vec2f(mInitialPos.x + (mEndPos.x - mStartPos.x), mInitialPos.y);
        
        if (newPosition.x < 0) {
            newPosition = ci::Vec2f(0, mInitialPos.y);
        }
        
        if (newPosition.x > (mTrack->getWidth() - mHandle->getWidth())) {
            newPosition = ci::Vec2f(mInitialPos.x, mTrack->getHeight() - mHandle->getHeight());
            newPosition = ci::Vec2f(mTrack->getWidth() - mHandle->getWidth(), mInitialPos.y);
        }
        
        //	Set the new position
        mHandle->setPosition(newPosition);
        
        //  Determine percentage
        float pct = newPosition.x / mFullLength;
        std::cout << "Scrubber::onMouseDrag: pct: " << pct << std::endl;
        mScrubberSignal(pct);
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
