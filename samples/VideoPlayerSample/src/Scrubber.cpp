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
    mTrack->setName("track");
    addChild(mTrack);
    
    //  create and add the scrub handle
    mHandle = Shape::createRect(50, 10);
    mHandle->setFillColor(1, 1, 1);
    mHandle->setAlpha(0.8f);
    mHandle->setName("handle");
    addChild(mHandle);
    
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

//void Scrubber::onMouseDown(MouseEvent &event)
//{
//    if (!mIsPressed) {
//        
//        std::cout << "Scrubber::onMouseDown: source: " << event.getSource()->getWidth() << std::endl;
//        mIsPressed = true;
//        
//        //	Store the initial positions
//        mInitialPos = mHandle->getPosition();
//        mStartPos = getParent()->windowToLocal(event.getWindowPos());
//        mEndPos = getParent()->windowToLocal(event.getWindowPos());
//        
//        //	Highlight the scroll thumb
//        mHandle->setAlpha(1.0);
//    }
//}

void Scrubber::onMouseDownHandle(MouseEvent &event)
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

void Scrubber::onMouseDownTrack(MouseEvent &event)
{
    if (!mIsPressed) {
        mIsPressed = true;
        
        ci::Vec2f clickPos = getParent()->windowToLocal(event.getWindowPos());
        
        float newXPos = clickPos.x - mHandle->getWidth() / 2;
        if (newXPos < 0) {
            newXPos = 0;
        } else if (newXPos > mTrack->getWidth() - mHandle->getWidth()) {
            newXPos = mTrack->getWidth() - mHandle->getWidth();
        }

        mHandle->setPosition(newXPos, 0);
        
        //  Determine percentage
        float pct = newXPos / mFullLength;
        mScrubberSignal(pct);
        
        //	With repositioned hadle, store the initial positions
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
