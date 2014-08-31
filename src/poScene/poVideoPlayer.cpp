//
//  poVideoPlayer.cpp
//  VideoPlayer
//
//  Created by Stephen Varga on 6/17/14.
//
//

#include "poVideoPlayer.h"


namespace po {
    template<class T>
    std::shared_ptr<VideoPlayer<T> > VideoPlayer<T>::create() {
        std::shared_ptr<VideoPlayer<T> > ref = std::shared_ptr<VideoPlayer<T> >(new VideoPlayer());
        ref->setup();
        return ref;
    }
    
    
    template<class T>
    std::shared_ptr<VideoPlayer<T> > VideoPlayer<T>::create(GenericMovieRef movieRef) {
        std::shared_ptr<VideoPlayer<T> > ref = std::shared_ptr<VideoPlayer<T> >(new VideoPlayer());
        ref->setup();
        ref->setMovieRef(movieRef);
        return ref;
    }
    
    
    template<class T>
    void VideoPlayer<T>::setup() {}
    
    
    template<class T>
    void VideoPlayer<T>::update() {}
    
    
    template<class T>
    ci::Rectf VideoPlayer<T>::getBounds()
    {
        GenericMovieRef m = mMovieRef.lock();
        if(m) return m->getBounds();
        return ci::Rectf(0,0,0,0);
    }
    
    
    template<class T>
    void VideoPlayer<T>::draw()
    {
        GenericMovieRef m = mMovieRef.lock();
        if(m && m->getTexture()) {
            ci::gl::color(ci::ColorA(getFillColor(), getAppliedAlpha()));
            ci::gl::draw(m->getTexture());
        }
    }
}