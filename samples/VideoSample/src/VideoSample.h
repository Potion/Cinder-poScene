#pragma once

#include "poNodeContainer.h"
#include "poVideo.h"

// In order to use poVideo, you must link the QuickTime.framework.
// In Xcode, you can add this under Targets -> BuildPhases -> Link Binary With Libraries.
// Hit [+] at the bottom, and choose QuickTime.framework from the list.


class VideoSample;
typedef std::shared_ptr<VideoSample> VideoSampleRef;

class VideoSample
: public po::scene::NodeContainer
{
public:
    static VideoSampleRef create();
    
    void setup();
    
protected:
    VideoSample();
    
private:
    po::scene::VideoGlRef       mVideo;
    void                        spinPlayer();
};
