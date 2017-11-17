#pragma once

#include "poScene/View.h"
#include "poScene/VideoView.h"

#include "PlayerButton.h"
#include "Scrubber.h"

class PlayerController;
typedef std::shared_ptr<PlayerController> PlayerControllerRef;

class PlayerController
: public po::scene::View {
    
public:
    static PlayerControllerRef  create();
    void                        setup();
    void                        setPrimaryMovie(po::scene::VideoViewGlRef video);
    
protected:
    PlayerController();
    //  override virtual function from NodeContainer
    void                    update();
    
private:
    po::scene::VideoViewGlRef   mVideoReference;
    float                   mCurrentDuration;

    void                    getPlaySignal();
    void                    getPauseSignal();
    void                    getScrubberSignal(float pct);
    
    PlayerButtonRef         mPlayButton, mPauseButton;
    ScrubberRef             mScrubber;
};