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
    
    VideoPlayerRef VideoPlayer::create(ci::qtime::MovieGlRef movieRef) {
        VideoPlayerRef ref = VideoPlayerRef(new VideoPlayer());
        ref->setup();
        ref->setMovieRef(movieRef);
        return ref;
    }
    
    VideoPlayer::VideoPlayer() {}
    
    void VideoPlayer::setup() {}
    
    void VideoPlayer::update()
    {
        ci::qtime::MovieGlRef m = mMovieRef.lock();
    }
    
    ci::Rectf VideoPlayer::getBounds()
    {
        ci::qtime::MovieGlRef m = mMovieRef.lock();
        if(m) return m->getBounds();
        return ci::Rectf(0,0,0,0);
    }
    
    void VideoPlayer::draw()
    {
        ci::qtime::MovieGlRef m = mMovieRef.lock();
        if(m && m->getTexture()) {
            ci::gl::color(ci::ColorA(getFillColor(), getAppliedAlpha()));
            ci::gl::draw(m->getTexture());
        }
    }
}