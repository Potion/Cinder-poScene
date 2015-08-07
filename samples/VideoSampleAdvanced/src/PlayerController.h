#pragma once

#include "poNodeContainer.h"
#include "poVideo.h"
#include "PlayerButton.h"
#include "Scrubber.h"

class PlayerController;
typedef std::shared_ptr<PlayerController> PlayerControllerRef;

class PlayerController
: public po::scene::NodeContainer {
    
public:
    static PlayerControllerRef  create();
    void                        setup();
    void                        setPrimaryMovie(po::scene::VideoGlRef video);
    
protected:
    PlayerController();
    //  override virtual function from NodeContainer
    void                    update();
    
private:
    po::scene::VideoGlRef   mVideoReference;
    float                   mCurrentDuration;

    void                    getPlaySignal();
    void                    getPauseSignal();
    void                    getScrubberSignal(float pct);
    
    PlayerButtonRef         mPlayButton, mPauseButton;
    ScrubberRef             mScrubber;
};