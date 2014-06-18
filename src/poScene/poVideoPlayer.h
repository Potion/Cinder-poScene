//
//  poVideoPlayer.h
//  VideoPlayer
//
//  Created by Stephen Varga on 6/17/14.
//
//

#pragma once

#include "poNode.h"

#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTime.h"

namespace po {
    class VideoPlayer;
    typedef std::shared_ptr<VideoPlayer> VideoPlayerRef;
    
    class VideoPlayer
    : public po::Node {
    public:
        static VideoPlayerRef create();
        
        void update();
        void draw();
        
        ci::Rectf getBounds();
        
        void load(const ci::fs::path &moviePath);
        void load(ci::DataSourceRef data);
        
        void play() { if(mVideo) mVideo->play(); };
        void stop();
        void setLoop(bool loop) { if(mVideo) mVideo->setLoop(loop); };
        
        
    protected:
        VideoPlayer();
        
    private:
        ci::qtime::MovieGlRef mVideo;
        ci::gl::Texture mVideoTex;
    };
}
