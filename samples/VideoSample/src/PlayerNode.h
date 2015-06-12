#pragma once

#include "poNodeContainer.h"
#include "poVideo.h"
#include "poShape.h"

// In order to use poVideo, you must link the QuickTime.framework.
// In Xcode, you can add this under Targets -> BuildPhases -> Link Binary With Libraries.
// Hit [+] at the bottom, and choose QuickTime.framework from the list.

class PlayerNode;
typedef std::shared_ptr<PlayerNode> PlayerNodeRef;

class PlayerNode
: public po::scene::NodeContainer {
    
public:
    static PlayerNodeRef    create();
    void                    setup();
    
protected:
    PlayerNode();
    //  override virtual function from NodeContainer
    void                    update();
    
private:
    po::scene::VideoGlRef   mVideoDisplayer;
    po::scene::ShapeRef     mForwardArrow;
    po::scene::ShapeRef     mBackwardArrow;

    bool                    mIsReversed;
    ci::Color               mActiveArrowColor;

    void                    onClickVideo();
    void                    onClickForward();
    void                    onClickBackward();
    
    void                    setColorsPlayingForward();
    void                    setColorsPlayingBackward();
    void                    setColorsNotPlaying();
};