#pragma once

#include "poNodeContainer.h"
#include "poVideo.h"


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
    void                        resetPlayerRotation();
};
