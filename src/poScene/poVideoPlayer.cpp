//
//  poVideoPlayer.cpp
//  VideoPlayer
//
//  Created by Stephen Varga on 6/17/14.
//
//

#include "poVideoPlayer.h"


namespace po {
    VideoPlayerRef VideoPlayer::create() {
        VideoPlayerRef ref = VideoPlayerRef(new VideoPlayer());
        ref->setup();
        return ref;
    }
    
    VideoPlayer::VideoPlayer()
    {
        
    }
    
    void VideoPlayer::update()
    {
        if(mVideo) mVideoTex = mVideo->getTexture();
    }
    
    ci::Rectf VideoPlayer::getBounds()
    {
        if(mVideo) return mVideo->getBounds();
        return ci::Rectf(0,0,0,0);
    }
    
    void VideoPlayer::draw()
    {
        if(mVideoTex) {
            ci::gl::color(getFillColor());
            ci::gl::draw(mVideoTex);
        }
    }
    
    void VideoPlayer::load(const ci::fs::path &moviePath)
    {
        try {
            mVideo = ci::qtime::MovieGl::create(moviePath);
        }
        catch( ... ) {
            ci::app::console() << "Unable to load the movie from location " << moviePath << std::endl;
            mVideo->reset();
            mVideoTex.reset();
        }
    }
    
    void VideoPlayer::load(const ci::DataSourceRef data)
    {
        try {
            mVideo = ci::qtime::MovieGl::create(data);
        }
        catch( ... ) {
            ci::app::console() << "Unable to load the movie from location " << data << std::endl;
            mVideo->reset();
            mVideoTex.reset();
        }
    }
}