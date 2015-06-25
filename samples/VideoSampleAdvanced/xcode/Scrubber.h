#pragma once

#include "poNodeContainer.h"
#include "poShape.h"

class Scrubber;
typedef std::shared_ptr<Scrubber> ScrubberRef;

class Scrubber
: public po::scene::NodeContainer {

public:
    typedef boost::signals2::signal<void(float pct)> ScrubberSignal;
    
    static ScrubberRef create();
    void setup();
    
    //	Get the thumb position for text positioning
    ci::Vec2f getHandlePosition() { return mHandle->getPosition(); }
    void setHandlePosition(float pct);
    ScrubberSignal &getScrubberSignal() { return mScrubberSignal; };
    
protected:
    Scrubber();
    
private:
    //	Scrub handle
    po::scene::ShapeRef mHandle;
    
    //	Scroll track
    po::scene::ShapeRef mTrack;
    
    //	Keep track of mouse position for dragging
    bool mIsPressed;
    ci::Vec2f mStartPos, mEndPos;
    ci::Vec2f mInitialPos;
    
    //	Mouse event handlers
    void onMouseDownHandle(po::scene::MouseEvent &event);
    void onMouseDownTrack(po::scene::MouseEvent &event);
    void onMouseDrag(po::scene::MouseEvent & event);
    void onMouseUp(po::scene::MouseEvent &event);
    
    ScrubberSignal  mScrubberSignal;
    float           mFullLength;
    
};