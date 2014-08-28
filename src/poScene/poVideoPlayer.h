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
        static VideoPlayerRef create(ci::qtime::MovieGlRef movieRef);
        
        void setup();
        void update();
        void draw();
        
        //Returns the bounds of our video (if we have one)
        ci::Rectf getBounds();
        
        ci::qtime::MovieGlRef getMovieRef() { return mMovieRef.lock(); };
        void setMovieRef(ci::qtime::MovieGlRef movieRef) { mMovieRef = movieRef; };
        void clearMovieRef();
        
    protected:
        VideoPlayer();
        
    private:
        //Movie and texture refs
        std::weak_ptr<ci::qtime::MovieGl> mMovieRef;
    };
}
