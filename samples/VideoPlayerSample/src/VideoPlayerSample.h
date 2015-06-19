#pragma once

#include "poNodeContainer.h"
#include "poShape.h"
#include "poVideo.h"
#include "PlayerController.h"
#include "MovieThumb.h"

// In order to use poVideo, you must link the QuickTime.framework.
// In Xcode, you can add this under Targets -> BuildPhases -> Link Binary With Libraries.
// Hit [+] at the bottom, and choose QuickTime.framework from the list.

class VideoPlayerSample;
typedef std::shared_ptr<VideoPlayerSample> VideoPlayerSampleRef;

class VideoPlayerSample
: public po::scene::NodeContainer
{
public:
    static VideoPlayerSampleRef create();
    
    void setup();
    
protected:
	VideoPlayerSample();

private:
    
    PlayerControllerRef     mPlayer;
    MovieThumbRef           mMovies[3];

    const int               mNumMovies;
    void                    setUpMovies();
    void                    onThumbnailClick(po::scene::MouseEvent &event);
    void                    onAnimationComplete(MovieThumbRef thumbnail);
    void                    animateControllerToPos(MovieThumbRef movie);
    
    ci::Vec2f               mPrimaryDisplayerPosition;
};
