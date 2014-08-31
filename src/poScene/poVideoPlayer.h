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
        static std::shared_ptr<VideoPlayer<T> > create() {
            std::shared_ptr<VideoPlayer<T> > ref = std::shared_ptr<VideoPlayer<T> >(new VideoPlayer());
            ref->setup();
            return ref;
        }
        
        static std::shared_ptr<VideoPlayer<T> > create(GenericMovieRef movieRef) {
            std::shared_ptr<VideoPlayer<T> > ref = std::shared_ptr<VideoPlayer<T> >(new VideoPlayer());
            ref->setup();
            ref->setMovieRef(movieRef);
            return ref;
        }
        
        void setMovieRef(GenericMovieRef movieRef)
        {
            mMovieRef = movieRef;
        };
        
        GenericMovieRef getMovieRef() { return mMovieRef; };
        
        ci::Rectf getBounds()
        {
            GenericMovieRef m = mMovieRef.lock();
            if(m) return m->getBounds();
            return ci::Rectf(0,0,0,0);
        }
        
    protected:
        VideoPlayer() {}
        
        void setup() {}
        void update(){}
        
        void draw()
        {
            GenericMovieRef m = mMovieRef.lock();
            if(m && m->getTexture()) {
                ci::gl::color(ci::ColorA(getFillColor(), getAppliedAlpha()));
                ci::gl::draw(m->getTexture());
            }
        }
        
    private:
        
        //Movie and texture refs
        std::weak_ptr<T> mMovieRef;
    };
    
    template<class T> using VideoPlayerRef = std::shared_ptr<VideoPlayer<T> >;
}
