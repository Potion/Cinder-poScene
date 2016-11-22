#pragma once

#include "poScene/View.h"
#include "poScene/VideoView.h"


class VideoSample;
typedef std::shared_ptr<VideoSample> VideoSampleRef;

class VideoSample
: public po::scene::View
{
public:
    static VideoSampleRef create();
    
    void setup();
    
protected:
    VideoSample();
    
private:
    po::scene::VideoViewGlRef       mVideo;
    void                        spinPlayer();
    void                        resetPlayerRotation();
};
