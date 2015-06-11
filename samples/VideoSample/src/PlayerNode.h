//
//  PlayerNode.h
//  VideoSample
//
//  Created by Jennifer Presto on 6/10/15.
//
//

#pragma once

#include "poNodeContainer.h"
#include "poVideo.h"

// In order to use poVideo, you must link the QuickTime.framework.
// You can add this under Targets < BuildPhases < Link Binary With Libraries.
// Hit [+] at the bottom, and choose QuickTime.Framework from the list.

class PlayerNode;
typedef std::shared_ptr<PlayerNode> PlayerNodeRef;

class PlayerNode
: public po::scene::NodeContainer {
    
public:
    static PlayerNodeRef create();
    void setup();
    
protected:
    void update();
    
private:
    po::scene::VideoGlRef mVideoDisplayer;
    void onMouseClick();
    
};