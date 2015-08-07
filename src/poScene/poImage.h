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
#include "cinder/gl/draw.h"
namespace po { namespace scene {
	// Images are simple wrappers that draw a ci::gl::Texture to a
    // quad at their actual size.
    // The Node attributes (i.e. position, scale, rotation, animation,  etc.) can then be applied
    //
    // For more complex usage of textures (i.e. mapping to complex shapes)
    // use the Shape class.
    
    class Image;
    typedef std::shared_ptr<Image> ImageRef;
    
    class Image
    : public Node
    {
    public:
        //! Create an empty image (texture is a null_ptr, won't draw until set)
        static ImageRef create();
        //! Create an image with a ci::gl::TextureRef
        static ImageRef create(ci::gl::TextureRef texture);
        
        //! Draw the texture if set
        void draw();
        
        //! Set the ci::gl::TextureRef
        void setTexture(ci::gl::TextureRef texture) { mTexture = texture; }
        //! Get the ci::gl::TextureRef
        ci::gl::TextureRef getTexture() { return mTexture; }
        
        //! Get the bounds of the Image
        ci::Rectf getBounds();
        
    protected:
        Image(ci::gl::TextureRef texture);
        
    private:
        ci::gl::TextureRef mTexture;
		
    };

} } //  namespace po::scene