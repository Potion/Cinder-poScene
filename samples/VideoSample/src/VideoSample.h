#pragma once

#include "poNodeContainer.h"
#include "PlayerNode.h"

// In order to use poVideo, you must link the QuickTime.framework.
// You can add this under Targets < BuildPhases < Link Binary With Libraries.
// Hit [+] at the bottom, and choose QuickTime.Framework from the list.
 
class VideoSample;
typedef std::shared_ptr<VideoSample> VideoSampleRef;

class VideoSample
: public po::scene::NodeContainer
{
public:
    static VideoSampleRef create();
    
    void setup();
    
protected:
	
private:
    PlayerNodeRef mVideoPlayer;
    
    void rotateVideoPlayerClockwise();
    void rotateVideoPlayerCounterClockwise();
    
    void simplifyVideoRotationValue();
    
};
