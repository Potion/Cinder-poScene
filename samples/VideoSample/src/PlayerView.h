#pragma once

#include "poScene/View.h"
#include "poScene/VideoView.h"

// In order to use poVideo, you must link the QuickTime.framework.
// In Xcode, you can add this under Targets -> BuildPhases -> Link Binary With Libraries.
// Hit [+] at the bottom, and choose QuickTime.framework from the list.

class PlayerView;
typedef std::shared_ptr<PlayerView> PlayerViewRef;

class PlayerView
: public po::scene::View {
    
public:
    static PlayerViewRef    create();
    void                    setup();
    
protected:
    PlayerView();
    //  override virtual function from NodeContainer
    void                    update();
    
private:
    po::scene::VideoViewGlRef   mVideoDisplayer;
    void						clickVideo();
	ci::gl::Texture2dRef		mTexture;
};