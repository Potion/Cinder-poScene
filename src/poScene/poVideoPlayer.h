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
        
        //Returns the bounds of our video (if we have one)
        ci::Rectf getBounds();
        
        void load(const ci::fs::path &moviePath);
        void load(ci::DataSourceRef data);
        
        void unload();
        
        ci::qtime::MovieGlRef getMovieRef();
        
        //Convenience functions
        void play()     { if(mVideo) mVideo->play();    };
        void stop()     { if(mVideo) mVideo->stop();    };
        bool isPlaying() { if(mVideo) return mVideo->isPlaying(); return false; }
        void setLoop(bool loop) { if(mVideo) mVideo->setLoop(loop); };
        ci::gl::Texture getTexture() { return mVideoTex; };
		bool isDone() { if(mVideo) return mVideo->isDone(); return false; }
		float getCurrentTime() { return mVideo->getCurrentTime(); }
		float getDuration() { return mVideo->getDuration(); }
		void seekToTime( float seconds ) { mVideo->seekToTime(seconds); }
		void seekToStart() { mVideo->seekToStart(); }
		void seekToEnd() { mVideo->seekToEnd(); }
		void setIsLoop(bool isLoop) { mVideo->setLoop(isLoop); }
        
    protected:
        VideoPlayer();
        
    private:
        //Movie and texture refs
        ci::qtime::MovieGlRef mVideo;
        ci::gl::Texture mVideoTex;
    };
}
