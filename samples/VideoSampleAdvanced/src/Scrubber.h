#pragma once

#include "cinder/Signals.h"

#include "poScene/View.h"
#include "poScene/ShapeView.h"

class Scrubber;
typedef std::shared_ptr<Scrubber> ScrubberRef;

class Scrubber
: public po::scene::View {

public:
    typedef ci::signals::Signal<void(float pct)> ScrubberSignal;
    
    static ScrubberRef create();
    void setup() override;
    
    //	Get the thumb position for text positioning
    ci::vec2 getHandlePosition() { return mHandle->getPosition(); }
    void setHandlePosition(float pct);
    ScrubberSignal &getScrubberSignal() { return mScrubberSignal; };
    
protected:
    Scrubber();
    
private:
    //	Scrub handle
    po::scene::ShapeViewRef mHandle;
    
    //	Scroll track
    po::scene::ShapeViewRef mTrack;
    
    //	Keep track of mouse position for dragging
    bool mIsPressed;
    ci::vec2 mStartPos, mEndPos;
    ci::vec2 mInitialPos;
    
    //	Mouse event handlers
    void onMouseDownHandle(po::scene::MouseEvent &event);
    void onMouseDownTrack(po::scene::MouseEvent &event);
    void onMouseDrag(po::scene::MouseEvent & event);
    void onMouseUp(po::scene::MouseEvent &event);
    
    ScrubberSignal  mScrubberSignal;
    float           mFullLength;
    
};
