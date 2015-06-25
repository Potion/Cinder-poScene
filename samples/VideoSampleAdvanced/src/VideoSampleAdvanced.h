#pragma once

#include "poNodeContainer.h"
#include "poShape.h"
#include "poVideo.h"
#include "PlayerController.h"
#include "MovieThumb.h"

// In order to use poVideo, you must link the QuickTime.framework.
// In Xcode, you can add this under Targets -> BuildPhases -> Link Binary With Libraries.
// Hit [+] at the bottom, and choose QuickTime.framework from the list.

class VideoSampleAdvanced;
typedef std::shared_ptr<VideoSampleAdvanced> VideoSampleAdvancedRef;

class VideoSampleAdvanced
: public po::scene::NodeContainer
{
public:
    static VideoSampleAdvancedRef create();
    
    void setup();
    
protected:
    VideoSampleAdvanced();
    
private:
    
    PlayerControllerRef     mPlayer;
    MovieThumbRef           mMovies[3];
    
    const int               mNumMovies;
    void                    setUpMovies();
    void                    onThumbnailClick(po::scene::MouseEvent &event);
    void                    onAnimationComplete(MovieThumbRef thumbnail);
    void                    animateControllerToPos(MovieThumbRef movie);
    
    ci::Vec2f               mPrimaryDisplayerPosition;
    
    bool                    mIsControllerInPosition;
};
