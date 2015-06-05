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

#include "poNode.h"
#include "boost/any.hpp"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTime.h"

namespace po { namespace scene {
	
    template<class T>
    class Video
    : public po::Node
	{
    private:
        typedef std::shared_ptr<T> GenericMovieRef;
        
    public:
        static std::shared_ptr<Video<T> > create();
        static std::shared_ptr<Video<T> > create(GenericMovieRef movieRef);
        
        void setMovieRef(GenericMovieRef movieRef)  { mMovieRef = movieRef; };
        GenericMovieRef getMovieRef()               { return mMovieRef; };
        
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
	
    //  Class Implementation
    template<class T>
    std::shared_ptr<Video<T> > Video<T>::create()
	{
        std::shared_ptr<Video<T> > ref = std::shared_ptr<Video<T> >(new Video());
        ref->setup();
        return ref;
    }
	
    template<class T>
    std::shared_ptr<Video<T> > Video<T>::create(GenericMovieRef movieRef)
	{
        std::shared_ptr<Video<T> > ref = std::shared_ptr<Video<T> >(new Video());
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
        return ci::Rectf(0,0,0,0);
    }
	
    template<class T>
    void Video<T>::draw()
    {
        if ( mMovieRef != nullptr && mMovieRef->getTexture() ) {
            ci::gl::color(ci::ColorA(getFillColor(), getAppliedAlpha()));
            ci::gl::draw(mMovieRef->getTexture());
        }
    }
    
    //	Template ref and GL ref
    template<class T> using VideoRef = std::shared_ptr<Video<T> >;
    
    typedef Video<ci::qtime::MovieGl> VideoGl;
    typedef std::shared_ptr<VideoGl> VideoGlRef;
	
} } //  namespace po::scene
