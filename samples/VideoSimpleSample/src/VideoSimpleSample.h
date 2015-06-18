#pragma once

#include "poNodeContainer.h"
#include "PlayerNode.h"

// In order to use poVideo, you must link the QuickTime.framework.
// In Xcode, you can add this under Targets -> BuildPhases -> Link Binary With Libraries.
// Hit [+] at the bottom, and choose QuickTime.framework from the list.


class VideoSimpleSample;
typedef std::shared_ptr<VideoSimpleSample> VideoSimpleSampleRef;

class VideoSimpleSample
: public po::scene::NodeContainer
{
public:
    static VideoSimpleSampleRef create();
    
    void setup();
    
protected:
	VideoSimpleSample();

private:
    PlayerNodeRef               mPlayer;
    void                        spinPlayer();

};
