//
//  poVideo.h
//  Video
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
    class Video
    : public po::Node {
        
    private:
        typedef std::shared_ptr<T> GenericMovieRef;
        
    public:
        static std::shared_ptr<Video<T> > create();
        static std::shared_ptr<Video<T> > create(GenericMovieRef movieRef);
        
        void setMovieRef(GenericMovieRef movieRef)  { mMovieRef = movieRef; };
        GenericMovieRef getMovieRef()               { return mMovieRef; };
        
        ci::Rectf getBounds();
        
    protected:
        Video() {}
        
        void setup();
        void update();
        
        void draw();
        
    private:
        //Movie and texture refs
        std::weak_ptr<T> mMovieRef;
    };
    
    //Template ref and GL ref
    template<class T> using VideoRef = std::shared_ptr<Video<T> >;
    
    typedef Video<ci::qtime::MovieGl> VideoGl;
    typedef std::shared_ptr<VideoGl> VideoGlRef;
}

#include "poVideo.ipp"
