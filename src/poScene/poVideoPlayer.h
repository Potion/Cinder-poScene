//
//  poVideoPlayer.h
//  VideoPlayer
//
//  Created by Stephen Varga on 6/17/14.
//
//

#pragma once

#include "poNode.h"

#include "boost/any.hpp"

#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTime.h"

namespace po {
    template<class T>
    class VideoPlayer
    : public po::Node {
        
    private:
        typedef std::shared_ptr<T> GenericMovieRef;
        
    public:
        static std::shared_ptr<VideoPlayer<T> > create();
        static std::shared_ptr<VideoPlayer<T> > create(GenericMovieRef movieRef);
        
        void setMovieRef(GenericMovieRef movieRef)  { mMovieRef = movieRef; };
        GenericMovieRef getMovieRef()               { return mMovieRef; };
        
        ci::Rectf getBounds();
        
    protected:
        VideoPlayer() {}
        
        void setup();
        void update();
        
        void draw();
        
    private:
        //Movie and texture refs
        std::weak_ptr<T> mMovieRef;
    };
    
    template<class T> using VideoPlayerRef = std::shared_ptr<VideoPlayer<T> >;    
}

#include "poVideoPlayer.cpp"
