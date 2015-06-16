#pragma once

#include "poNodeContainer.h"
#include "poVideo.h"

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
    po::scene::VideoGlRef   mMovie1;
    po::scene::VideoGlRef   mMovie2;
    po::scene::VideoGlRef   mMovie3;
    
    void                    setUpMovieThumbnails();
    

};
