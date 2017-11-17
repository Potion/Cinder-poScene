/*
 Copyright (c) 2015, Potion Design LLC
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 * Neither the name of copyright holder nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "cinder/qtime/QuickTime.h"
#if ! defined(  CINDER_MSW ) && ! defined( CINDER_LINUX )
    #include "cinder/qtime/QuickTimeGlImplAvf.h"
#endif
#include "cinder/qtime/QuickTimeGl.h"
#include "cinder/gl/wrapper.h"
#include "cinder/gl/draw.h"
#include "poNode.h"
#include "boost/any.hpp"
#include "cinder/app/App.h"
#include "cinder/gl/Texture.h"

namespace po { namespace scene {
    // This class is a generic wrapper for any Cinder based video player
    // that provides a "getBounds() and getTexture()" function.
    //
    // It is designed to be as flexible as possible given the numerous Cinder video
    // players that exist, and provide a po::scene::VideoGl type that uses
    // the Cinder Quicktime player (currently the most standard player in 0.8.6)
	
    template<class T>
    class Video
    : public Node
	{
    private:
        typedef std::shared_ptr<T> GenericMovieRef;
        
    public:
        //! Create empty Video node
        static std::shared_ptr< Video<T> > create();
        //! Create with a movieref
        static std::shared_ptr< Video<T> > create(GenericMovieRef movieRef);
        //! Set the movie ref
        void setMovieRef(GenericMovieRef movieRef)  { mMovieRef = movieRef; };
        //! Get the movie ref
        GenericMovieRef getMovieRef()               { return mMovieRef; };
        //! Get the bounds (based on the movie's bounds)
        ci::Rectf getBounds();
        
    protected:
		Video() {};
		
        void setup();
        void update();
        void draw();
        
    private:
        //	Movie and texture refs
        std::shared_ptr<T> mMovieRef;
    };
	
    //  Template Class Implementation
    template<class T>
    std::shared_ptr< Video<T> > Video<T>::create()
	{
        std::shared_ptr< Video<T> > ref = std::shared_ptr< Video<T> >(new Video());
        ref->setup();
        return ref;
    }
	
    template<class T>
    std::shared_ptr< Video<T> > Video<T>::create(GenericMovieRef movieRef)
	{
        std::shared_ptr< Video<T> > ref = std::shared_ptr< Video<T> >(new Video());
        ref->setup();
        ref->setMovieRef(movieRef);
        return ref;
    }
	
    template<class T>
    void Video<T>::setup() {}
	
    template<class T>
    void Video<T>::update() {}
	
    template<class T>
    ci::Rectf Video<T>::getBounds()
    {
        if (mMovieRef != nullptr) return mMovieRef->getBounds();
        return ci::Rectf(0, 0, 0, 0);
    }
	
    template<class T>
    void Video<T>::draw()
    {
        if ( mMovieRef != nullptr && mMovieRef->getTexture() ) {
			ci::gl::ScopedBlendAlpha alphaBlendScoped;
			ci::gl::ScopedColor fillColorScoped(ci::ColorA(getFillColor(), getAppliedAlpha()));
            ci::gl::draw(mMovieRef->getTexture());
        }
    }
    
    //	Template ref and GL ref
    template<class T> using VideoRef = std::shared_ptr< Video<T> >;
    
    typedef Video<ci::qtime::MovieGl> VideoGl;
    typedef std::shared_ptr<VideoGl> VideoGlRef;
	
} } //  namespace po::scene
