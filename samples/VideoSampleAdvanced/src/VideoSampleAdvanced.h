#pragma once

#include "poScene/View.h"
#include "poScene/ShapeView.h"
#include "poScene/VideoView.h"

#include "PlayerController.h"
#include "MovieThumb.h"

class VideoSampleAdvanced;
typedef std::shared_ptr<VideoSampleAdvanced> VideoSampleAdvancedRef;

class VideoSampleAdvanced
: public po::scene::View
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
    
    ci::vec2                mPrimaryDisplayerPosition;
    
    bool                    mIsControllerInPosition;
};
