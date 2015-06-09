#pragma once

#include "poNodeContainer.h"

class VideoPlayer;
typedef std::shared_ptr<VideoPlayer> VideoPlayerRef;

class VideoPlayer
: public po::NodeContainer
{
public:
    static VideoPlayerRef create();
    
    void setup();
    
    #pragma message "Should we declare all events for this template?"
protected:
};
